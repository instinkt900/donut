#pragma once

#ifdef USE_OPENGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#else
#error TODO
#endif

#include <imgui.h>
