#pragma once

#include "vulkan_capsset.h"

namespace donut::vulkan {
	enum class ShaderCaps : uint32_t {
		Color = 0,
		VertexColor,
		PointSize,
		Billboard,
		Lighting,
		Texture0,
		Texture1,
		NormalMap,
		SpecularMap,
		Skinning,
		Count
	};

	typedef CapsSet<ShaderCaps> ShaderCapsSet;
}
