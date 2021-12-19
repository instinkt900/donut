#include "donut_pch.h"
#include "vulkan_shaderresource.h"

namespace donut::vulkan {
	const std::map<std::string, ShaderTextureName> SHADER_TEXTURE_NAMES = 
	{
		{ "texture0", ShaderTextureName::Texture0 },
		{ "texture1", ShaderTextureName::Texture1 },
		{ "normal_map", ShaderTextureName::NormalMap },
		{ "specular_map", ShaderTextureName::SpecularMap },
		{ "shadow_map", ShaderTextureName::ShadowMap },
		{ "shadow_map_atlas", ShaderTextureName::ShadowMapAtlas },
		{ "projector_texture", ShaderTextureName::ProjectorTexture },
		{ "environment_cubemap", ShaderTextureName::EnvironmentCubemap },
		{ "radiance_cubemap", ShaderTextureName::RadianceCubemap },
		{ "irradiance_cubemap", ShaderTextureName::IrradianceCubemap },
		{ "brdf_lut", ShaderTextureName::BrdfLUT },
	};

	const std::map<std::string, ShaderBufferName> SHADER_BUFFER_NAMES = 
	{
		{ "ObjectParams", ShaderBufferName::ObjectParams },
		{ "Camera", ShaderBufferName::Camera },
		{ "EnvironmentSettings", ShaderBufferName::EnvironmentSettings },
		{ "SkinningMatrices", ShaderBufferName::SkinningMatrices },
		{ "Lights", ShaderBufferName::Light },
		{ "Projectors", ShaderBufferName::Projector },
		{ "LightGrid", ShaderBufferName::LightGrid },
		{ "LightIndices", ShaderBufferName::LightIndices }
	};

	const std::map<std::string, ShaderSamplerName> SHADER_SAMPLER_NAMES = 
	{
		{ "SamplerLinearWrap", ShaderSamplerName::LinearWrap},
		{ "SamplerLinearClamp", ShaderSamplerName::LinearClamp},
		{ "SamplerPointWrap", ShaderSamplerName::PointWrap},
		{ "SamplerPointClamp", ShaderSamplerName::PointClamp},
	};

	const std::set<ShaderBufferName> SHADER_DYNAMIC_OFFSET_BUFFERS =
	{
		ShaderBufferName::ObjectParams,
		ShaderBufferName::Camera,
		ShaderBufferName::SkinningMatrices
	};
}
