#pragma once

#include "spirv_glsl.hpp"
#include "vulkan_shaderresource.h"
#include "vulkan_vertexlayout.h"

namespace donut::vulkan {
	class ReflectionInfo
	{
	public:
		struct EntryPoint
		{
			std::string name;
		};

		struct SamplerData
		{
			uint32_t id;
			std::string name;
			ShaderSamplerName sampler_name;
			unsigned set;
			unsigned binding;
		};

		struct UniformBufferData
		{
			uint32_t id;
			std::string name;
			unsigned set;
			unsigned binding;
			ShaderBufferName shader_buffer;
		};

		struct VertexAttribData
		{
			uint32_t id;
			std::string name;
			unsigned set;
			unsigned location;
			VertexAttrib vertex_attrib;
		};

		struct CombinedImageSamplerData
		{
			uint32_t id;
			std::string name;
			unsigned set;
			unsigned binding;
			ShaderTextureName shader_texture = ShaderTextureName::Unknown;
		};

		struct SeparateImageData
		{
			uint32_t id;
			std::string name;
			unsigned set;
			unsigned binding;
			ShaderTextureName shader_texture = ShaderTextureName::Unknown;
		};

		struct StorageBufferData
		{
			uint32_t id;
			std::string name;
			unsigned set;
			unsigned binding;
			ShaderBufferName storage_buffer_name = ShaderBufferName::Unknown;
		};

		struct PushConstantsData
		{
			uint32_t id;
			uint32_t offset;
			uint32_t size;
			std::string name;
		};

		ReflectionInfo(uint32_t* spirv_data, size_t count);

		const std::vector<UniformBufferData>& UniformBuffers() const { return m_uniformBuffers; }
		const std::vector<StorageBufferData>& StorageBuffers() const { return m_storageBuffers; }
		const std::vector<VertexAttribData>& VertexAttribs() const { return m_vertexAttribs; }
		const std::vector<CombinedImageSamplerData>& CombinedImageSamplers() const { return m_combinedImageSamplers; }
		const std::vector<PushConstantsData>& PushConstants() const { return m_pushConstants; }
		const std::vector<EntryPoint>& EntryPoints() const { return m_entryPoints; }
		const std::vector<SamplerData>& Samplers() const { return m_samplers; }
		const std::vector<SeparateImageData>& SeparateImages() const { return m_separateImages; }

	private:
		spirv_cross::CompilerGLSL m_compiler;
		std::vector<UniformBufferData> m_uniformBuffers;
		std::vector<VertexAttribData> m_vertexAttribs;
		std::vector<StorageBufferData> m_storageBuffers;
		std::vector<CombinedImageSamplerData> m_combinedImageSamplers;
		std::vector<SamplerData> m_samplers;
		std::vector<SeparateImageData> m_separateImages;
		std::vector<PushConstantsData> m_pushConstants;
		std::vector<EntryPoint> m_entryPoints;
	};

}