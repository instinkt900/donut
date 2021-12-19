cmake_minimum_required(VERSION 3.10)

project(donut)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES
	src/renderer/renderer_apis/vulkan/vulkan_context.cpp
	src/renderer/renderer_apis/vulkan/vulkan_pipeline.cpp
	src/renderer/renderer_apis/vulkan/vulkan_reflectioninfo.cpp
	src/renderer/renderer_apis/vulkan/vulkan_rendermode.cpp
	src/renderer/renderer_apis/vulkan/vulkan_renderpass.cpp
	src/renderer/renderer_apis/vulkan/vulkan_shader.cpp
	src/renderer/renderer_apis/vulkan/vulkan_shadercache.cpp
	src/renderer/renderer_apis/vulkan/vulkan_shadercompiler.cpp
	src/renderer/renderer_apis/vulkan/vulkan_shaderdefines.cpp
	src/renderer/renderer_apis/vulkan/vulkan_shaderresource.cpp
	src/renderer/renderer_apis/vulkan/vulkan_swapchain.cpp
	src/renderer/renderer_apis/vulkan/vulkan_utils.cpp)

add_library(donut ${SOURCE_FILES})

add_definitions()
target_include_directories(donut PUBLIC D:/Development/donut/packages/glfw.3.3.2/build/native/include)
target_include_directories(donut PUBLIC C:/VulkanSDK/1.2.170.0/Include)
target_include_directories(donut PUBLIC external/glm)
target_include_directories(donut PUBLIC external/spdlog/include)
target_include_directories(donut PUBLIC external/spdlog/include)
target_include_directories(donut PUBLIC external/entt/single_include)
target_include_directories(donut PUBLIC external/FastNoise2/include)
target_include_directories(donut PUBLIC external/VulkanMemoryAllocator/include)
target_include_directories(donut PUBLIC external/SPIRV-Cross)
target_include_directories(donut PUBLIC src/glad/include)
target_include_directories(donut PUBLIC src/imgui)
target_include_directories(donut PUBLIC src)
target_include_directories(donut PUBLIC external/concurrentqueue)
target_include_directories(donut PUBLIC external/DirectXShaderCompiler/include)

