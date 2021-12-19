#pragma once

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "noncopyable.h"
#include "abstract.h"

#include <glad/glad.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vk_mem_alloc.h>

#include <imgui.h>

#include <optional>
#include <set>
#include <map>

#include <cstdint>
#include <algorithm>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "renderer/iwindow.h"

//#include "renderer/shadersection.h"
//#include "renderer/shader.h"
// #include "renderer/vertexbuffer.h"
// #include "renderer/indexbuffer.h"
// #include "renderer/image.h"
// #include "renderer/texture2d.h"
// #include "renderer/framebuffer.h"
// #include "renderer/renderer.h"
// #include "renderer/iwindow.h"
// #include "renderer/ilayer.h"
// #include "renderer/layerstack.h"
// #include "renderer/window.h"

// #include "scene/scene.h"
// #include "scene/entity.h"
// #include "scene/systems/camerasystem.h"
// #include "scene/systems/inputsystem.h"
// #include "scene/systems/renderingsystem.h"

#include "application.h"
