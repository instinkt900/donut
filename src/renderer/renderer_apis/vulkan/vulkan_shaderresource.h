#pragma once

#include <map>
#include <set>
#include <string>

namespace donut::vulkan {
	enum class ShaderTextureName : uint32_t 
	{
		Texture0,
		Texture1,
		NormalMap,
		SpecularMap,
		ShadowMap,
		ShadowMapAtlas,
		ProjectorTexture,
		EnvironmentCubemap,
		RadianceCubemap,
		IrradianceCubemap,
		BrdfLUT,
		Unknown,
		Count
	};

	enum class ShaderBufferName : uint32_t 
	{
		ObjectParams,
		Camera,
		EnvironmentSettings,
		SkinningMatrices,
		Light,
		Projector,
		LightIndices, // SSBO
		LightGrid, // SSBO
		Unknown,
		Count
	};

	enum class ShaderSamplerName : uint32_t
	{
		LinearWrap,
		LinearClamp,
		PointWrap,
		PointClamp,
		Count
	};

	extern const std::map<std::string, ShaderTextureName> SHADER_TEXTURE_NAMES;
	extern const std::map<std::string, ShaderBufferName> SHADER_BUFFER_NAMES;
	extern const std::map<std::string, ShaderSamplerName> SHADER_SAMPLER_NAMES;
	extern const std::set<ShaderBufferName> SHADER_DYNAMIC_OFFSET_BUFFERS;
}
