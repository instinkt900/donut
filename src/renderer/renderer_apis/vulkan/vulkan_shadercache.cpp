#include "donut_pch.h"
#include "vulkan_shadercache.h"

#include "vulkan_utils.h"
#include "vulkan_shaderdefines.h"
#include "vulkan_shadercompiler.h"

#include "utils/file_utils.h"
#include "utils/memory_utils.h"

#include "system/job.h"

namespace donut::vulkan {

	const char* ShaderCache::shader_cache_dir = "shaders_compiled";

	ShaderProgramInfo& ShaderProgramInfo::AddShader(
		ShaderProgram::Stage stage, std::string path, std::string entry_point, std::vector<ShaderProgramInfo::Macro> defines
	)
	{
		shaders.push_back({ stage, path, std::move(entry_point), std::move(defines) });
		return *this;
	}

	ShaderProgramInfo& ShaderProgramInfo::AddShader(ShaderData&& shader_data)
	{
		shaders.emplace_back(std::move(shader_data));
		return *this;
	}

	ShaderProgramInfo::ShaderData::ShaderData(ShaderProgram::Stage stage, std::string path, std::string entry_point, std::vector<Macro> defines)
		: stage(stage), path(path), entry_point(entry_point), defines(std::move(defines))
	{
	}

	const ShaderProgramInfo::ShaderData* ShaderProgramInfo::GetShaderData(ShaderProgram::Stage stage) const
	{
		for (auto& data : shaders)
			if (data.stage == stage)
				return &data;

		return nullptr;
	}

	void ShaderCache::AppendCapsDefines(const ShaderCapsSet& caps_set, std::vector<ShaderProgramInfo::Macro>& out_defines)
	{
		if (caps_set.getBitmask() == 0) return;

		for (int i = 0; i < (int)ShaderCaps::Count; i++)
			if (caps_set.hasCap((ShaderCaps)i))
				out_defines.push_back({ SHADER_CAPS_DEFINES.at((ShaderCaps)i), "1"});
	}

	uint32_t ShaderCache::GetDefinesHash(std::vector<ShaderProgramInfo::Macro> defines)
	{
		uint32_t hash = 0;
		if (defines.empty()) return hash;

		std::sort(defines.begin(), defines.end(), [](const ShaderProgramInfo::Macro& a, const ShaderProgramInfo::Macro& b) { return a.name < b.name; });

		for (size_t i = 0; i < defines.size(); i++)
		{
			auto& define = defines[i];
			auto name_hash = define.name.length() ? Hash(define.name.data(), define.name.length() * sizeof(char)) : 0u;
			auto value_hash = define.value.length() ? Hash(define.value.data(), define.value.length() * sizeof(char)) : 0u;
			uint32_t combined[] = { hash, name_hash, value_hash };
			hash = Hash(combined, sizeof(combined));
		}

		return hash;
	}

	uint32_t ShaderCache::GetCombinedHash(uint32_t name_hash, const ShaderCapsSet& caps_set)
	{
		uint32_t combined_hash[] = { name_hash, caps_set.getBitmask() };
		return Hash(combined_hash, sizeof(combined_hash));
	}

	uint32_t GetShaderSourceHash(const std::string& path)
	{
		auto source_data = LoadFile(path);
		uint32_t source_hash = 0;
		if (source_data.size())
			source_hash = Hash(source_data.data(), source_data.size());

		std::vector<uint32_t> all_hashes;
		all_hashes.push_back(source_hash);

		InputMemoryStream stream((char*)source_data.data(), source_data.size());
		std::string line;

		while (std::getline(stream, line))
		{
			if (line.find("#include \"") == 0)
			{
				auto last = line.rfind("\"");
				if (last != std::string::npos && last > 0)
				{
					std::string include_path = line.substr(10, last - 10);
					auto include_hash = GetShaderSourceHash("shaders/" + include_path);
					all_hashes.push_back(include_hash);
				}
			}
		}

		if (all_hashes.size() > 1)
		{
			source_hash = Hash(all_hashes.data(), all_hashes.size() * sizeof(uint32_t));
		}

		return source_hash;
	}

	uint32_t ShaderCache::GetShaderDataHash(const ShaderProgramInfo::ShaderData& data)
	{
		auto defines_hash = GetDefinesHash(data.defines);
		auto path_hash = Hash(data.path.data(), data.path.length() * sizeof(wchar_t));
		auto entry_point_hash = Hash(data.entry_point.data(), data.entry_point.length());

		const uint32_t source_hash = GetShaderSourceHash(data.path);
		const uint32_t hashes[] = { defines_hash, path_hash, entry_point_hash, source_hash };
		return Hash(hashes, sizeof(hashes));
	}

	uint32_t ShaderCache::GetShaderProgramInfoHash(const ShaderProgramInfo& program_info)
	{
		uint32_t vertex_hash = 0;
		uint32_t fragment_hash = 0;
		uint32_t compute_hash = 0;

		for (auto& data : program_info.shaders)
		{
			uint32_t hash = GetShaderDataHash(data);
			switch (data.stage)
			{
			case ShaderProgram::Stage::Vertex:
				vertex_hash = hash;
				break;
			case ShaderProgram::Stage::Fragment:
				fragment_hash = hash;
				break;
			case ShaderProgram::Stage::Compute:
				compute_hash = hash;
				break;
			default:
				throw std::runtime_error("unknown stage");
			}
		}

		return ShaderProgram::CalculateHash(fragment_hash, vertex_hash, compute_hash);
	}

	std::string ShaderCache::GetShaderCachePath(uint32_t shader_hash)
	{
		std::filesystem::path shader_path(shader_cache_dir);

		std::string filename = std::to_string(shader_hash);

		shader_path /= filename;
		shader_path.replace_extension(".spv");
		return shader_path.string();
	}

	ShaderCache::ShaderCache(Context& context)
	:m_context(context)
	{

	}

	ShaderCache::~ShaderCache() = default;

	ShaderModule* ShaderCache::GetShaderModule(const ShaderProgramInfo::ShaderData& shader_data)
	{
		auto hash = GetShaderDataHash(shader_data);

		std::scoped_lock lock(module_mutex);

		auto iter = module_cache.find(hash);
		if (iter != module_cache.end())
			return iter->second.get();

		auto shader_module = std::make_unique<ShaderModule>(hash);
		auto* result = shader_module.get();
		module_cache[hash] = std::move(shader_module);

		return result;
	}

	bool ShaderCache::LoadShaderModule(Context& context, ShaderModule& module, const ShaderProgramInfo::ShaderData& shader_data)
	{
		std::string filename = ShaderCache::GetShaderCachePath(module.GetHash());

		auto module_data = LoadFile(filename);
		if (!module_data.size())
		{
			ShaderCompiler::CompilationResult result;
			if (ShaderCompiler::CompileShader(*this, shader_data, result))
			{
				module_data = std::move(result.data);

				// TODO: save in a separate job
				std::error_code error;
				std::filesystem::create_directories(std::filesystem::path(filename).remove_filename(), error);

				if (!error)
				{
					// Saving to file
					std::ofstream stream(filename, std::ofstream::binary);
					stream.write((char*)module_data.data(), module_data.size());
				}
			}
			else
			{
				//std::cout << "Error compiling " << shader_data.path << std::endl << result.error << std::endl;
				throw std::runtime_error("Error compiling shader module");
			}
		}

		module.Load(context, module_data.data(), module_data.size());

		return true;
	}

	const std::vector<uint8_t>& ShaderCache::GetShaderSource(const std::string& filename)
	{
		{
			std::scoped_lock lock(source_mutex);
			auto it = source_cache.find(filename);
			if (it != source_cache.end() && it->second.size() > 0)
				return it->second;
		}

		auto source = LoadFile(filename);
		std::scoped_lock lock(source_mutex);
		auto it = source_cache.insert(std::make_pair(filename, source));
		return it.first->second;
	}

	class ShaderCache::PrepareShaderJob : public Job
	{
	public:
		PrepareShaderJob(ShaderCache* shader_cache, ShaderProgram* program, const ShaderProgramInfo::ShaderData* vertex_data, const ShaderProgramInfo::ShaderData* fragment_data, const ShaderProgramInfo::ShaderData* compute_data)
			: shader_cache(shader_cache)
			, program(program)
			, vertex_data(vertex_data ? new ShaderProgramInfo::ShaderData(*vertex_data) : nullptr)
			, fragment_data(fragment_data ? new ShaderProgramInfo::ShaderData(*fragment_data) : nullptr)
			, compute_data(compute_data ? new ShaderProgramInfo::ShaderData(*compute_data) : nullptr)
		{
		}

		virtual void Execute() override
		{
			if (vertex_data)
			{
				auto vertex_module = shader_cache->GetShaderModule(*vertex_data);
				if (vertex_module->TransitionState(ShaderModule::State::Unloaded, ShaderModule::State::Loading))
					shader_cache->LoadShaderModule(shader_cache->GetContext(), *vertex_module, *vertex_data);
				else
					vertex_module->WaitLoaded();

				program->AddModule(vertex_module, ShaderProgram::Stage::Vertex);
			}

			if (fragment_data)
			{
				auto fragment_module = shader_cache->GetShaderModule(*fragment_data);
				if (fragment_module->TransitionState(ShaderModule::State::Unloaded, ShaderModule::State::Loading))
					shader_cache->LoadShaderModule(shader_cache->GetContext(), *fragment_module, *fragment_data);
				else
					fragment_module->WaitLoaded();

				program->AddModule(fragment_module, ShaderProgram::Stage::Fragment);
			}

			if (compute_data)
			{
				auto compute_module = shader_cache->GetShaderModule(*compute_data);

				if (compute_module->TransitionState(ShaderModule::State::Unloaded, ShaderModule::State::Loading))
					shader_cache->LoadShaderModule(shader_cache->GetContext(), *compute_module, *compute_data);
				else
					compute_module->WaitLoaded();

				program->AddModule(compute_module, ShaderProgram::Stage::Compute);
			}

			program->Prepare(shader_cache->GetContext());
			bool state_transitioned = program->TransitionState(ShaderModule::State::Loading, ShaderModule::State::Loaded);
			assert(state_transitioned);
		}

	private:
		ShaderCache* shader_cache;
		ShaderProgram* program;
		std::unique_ptr<ShaderProgramInfo::ShaderData> vertex_data;
		std::unique_ptr<ShaderProgramInfo::ShaderData> fragment_data;
		std::unique_ptr<ShaderProgramInfo::ShaderData> compute_data;
	};

	ShaderProgram* ShaderCache::GetShaderProgram(const ShaderProgramInfo& info)
	{
		ShaderProgram* program;

		auto program_hash = GetShaderProgramInfoHash(info);
		{
			std::scoped_lock lock(program_mutex);
			auto iter = program_cache.find(program_hash);
			if (iter != program_cache.end())
				return iter->second.get();
			
			auto inserted = program_cache.insert(std::make_pair(program_hash, std::make_unique<ShaderProgram>()));
			program = inserted.first->second.get();
		}

		if (program->TransitionState(ShaderModule::State::Unloaded, ShaderModule::State::Loading))
		{
			auto* vertex_data = info.GetShaderData(ShaderProgram::Stage::Vertex);
			auto* fragment_data = info.GetShaderData(ShaderProgram::Stage::Fragment);
			auto* compute_data = info.GetShaderData(ShaderProgram::Stage::Compute);
			Scheduler::Get().SpawnJob<PrepareShaderJob>(Job::Priority::Low, this, program, vertex_data, fragment_data, compute_data);
		}

		return program;
	}
}
