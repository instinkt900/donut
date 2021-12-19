#include "donut_pch.h"
#include "vulkan_reflectioninfo.h"
#include "vulkan_shaderdefines.h"
#include "vulkan_shaderresource.h"

namespace donut::vulkan {

	std::string ConvertHLSLName(const std::string& str)
	{
		if (str.find("type_") == 0)
			return str.substr(5);

		if (str.find("in_var_") == 0)
		{
			std::string result = str.substr(7);
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
			return result;
		}

		return str;
	}

	ReflectionInfo::ReflectionInfo(uint32_t* spirv_data, size_t count)
		: m_compiler(spirv_data, count)
	{
		auto resources = m_compiler.get_shader_resources();
		for (auto& entry : m_compiler.get_entry_points_and_stages())
		{
			m_entryPoints.push_back({entry.name});
		}

		for (auto& sampler : resources.separate_samplers)
		{
			SamplerData data;
			data.id = sampler.id;
			data.name = sampler.name;
			data.set = m_compiler.get_decoration(sampler.id, spv::DecorationDescriptorSet);
			data.binding = m_compiler.get_decoration(sampler.id, spv::DecorationBinding);
			data.sampler_name = SHADER_SAMPLER_NAMES.at(sampler.name);

			m_samplers.push_back(data);
		}

		for (auto& ubo : resources.uniform_buffers)
		{
			UniformBufferData data;
			data.id = ubo.id;
			data.name = ubo.name;
			data.set = m_compiler.get_decoration(ubo.id, spv::DecorationDescriptorSet);
			data.binding = m_compiler.get_decoration(ubo.id, spv::DecorationBinding);
			data.shader_buffer = SHADER_BUFFER_NAMES.at(ConvertHLSLName(ubo.name)); // TODO: support for Unknown arbitrary buffer / sampler names

			m_uniformBuffers.push_back(data);
		}

		for (auto& input : resources.stage_inputs)
		{
			VertexAttribData data;
			data.id = input.id;
			data.name = input.name;
			data.set = m_compiler.get_decoration(input.id, spv::DecorationDescriptorSet);
			data.location = m_compiler.get_decoration(input.id, spv::DecorationLocation);
			auto attrib_iterator = VERTEX_ATTRIB_NAMES.find(ConvertHLSLName(data.name));
			data.vertex_attrib = attrib_iterator != VERTEX_ATTRIB_NAMES.end() ? attrib_iterator->second : VertexAttrib::Unknown; // unknown may be varyings (fragment input from vertex)
			m_vertexAttribs.push_back(data);
		}

		for (auto& sampler : resources.sampled_images)
		{
			CombinedImageSamplerData data;
			data.id = sampler.id;
			data.name = sampler.name;
			data.set = m_compiler.get_decoration(sampler.id, spv::DecorationDescriptorSet);
			data.binding = m_compiler.get_decoration(sampler.id, spv::DecorationBinding);
			auto iter = SHADER_TEXTURE_NAMES.find(ConvertHLSLName(data.name));
			if (iter != SHADER_TEXTURE_NAMES.end())
				data.shader_texture = iter->second;

			m_combinedImageSamplers.push_back(data);
		}

		for (auto& sampler : resources.separate_images)
		{
			SeparateImageData data;
			data.id = sampler.id;
			data.name = sampler.name;
			data.set = m_compiler.get_decoration(sampler.id, spv::DecorationDescriptorSet);
			data.binding = m_compiler.get_decoration(sampler.id, spv::DecorationBinding);
			auto iter = SHADER_TEXTURE_NAMES.find(ConvertHLSLName(data.name));
			if (iter != SHADER_TEXTURE_NAMES.end())
				data.shader_texture = iter->second;

			m_separateImages.push_back(data);
		}

		for (auto& storage_buffer : resources.storage_buffers)
		{
			StorageBufferData data;
			data.id = storage_buffer.id;
			data.name = storage_buffer.name;
			data.set = m_compiler.get_decoration(storage_buffer.id, spv::DecorationDescriptorSet);
			data.binding = m_compiler.get_decoration(storage_buffer.id, spv::DecorationBinding);
			auto iter = SHADER_BUFFER_NAMES.find(ConvertHLSLName(storage_buffer.name));
			if (iter != SHADER_BUFFER_NAMES.end())
				data.storage_buffer_name = iter->second;

			m_storageBuffers.push_back(data);
		}

		for (auto& push_constant_buffer : resources.push_constant_buffers)
		{
			PushConstantsData data;
			data.id = push_constant_buffer.id;
			data.name = push_constant_buffer.name;
			data.size = 0;
			auto ranges = m_compiler.get_active_buffer_ranges(data.id);
			for (auto &range : ranges)
			{
				data.size += static_cast<uint32_t>(range.range);
			}
			
			if (ranges.size())
			{
				data.offset = static_cast<uint32_t>(ranges[0].offset);
				m_pushConstants.push_back(data);
			}
		}
	}
}
