#pragma once

#include "vulkan_vertexlayout.h"
#include "vulkan_shadercaps.h"

namespace donut::vulkan {
	enum DescriptorSet : int
	{
		Global,
		Object
	};

	extern const std::map<std::string, VertexAttrib> VERTEX_ATTRIB_NAMES;
	extern const std::map<ShaderCaps, std::string> SHADER_CAPS_DEFINES;
}
