#include "donut_pch.h"
#if defined(USE_VULKAN)

#include "vulkan_glfw_window.h"

#include "renderer/events/eventresize.h"
#include "renderer/events/eventkey.h"
#include "renderer/events/eventmousebutton.h"
#include "renderer/events/eventmousescroll.h"
#include "renderer/events/eventmousemove.h"

#define GET_WINDOW(glfwWindow) static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow))

namespace donut::vulkan {
    std::shared_ptr<GLFWWindow> GLFWWindow::Create(int width, int height, std::string const& title) {
        if (!glfwInit()) {
            spdlog::error("glfwInit failed!");
            return nullptr;
        }

        glfwSetErrorCallback([](int error, char const* desc) {
            spdlog::error("glfw error: {}", desc);
        });

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        auto window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (nullptr == window) {
            spdlog::error("glfwCreateWindow failed!");
            return nullptr;
        }

        auto vkInstance = Renderer::GetInstance();
        VkSurfaceKHR surface;
        if (VK_SUCCESS != glfwCreateWindowSurface(vkInstance, window, nullptr, &surface)) {
            spdlog::error("glfwCreateWindowSurface failed!");
            return nullptr;
        }

        return std::shared_ptr<GLFWWindow>(new GLFWWindow(window, surface));
    }

    GLFWWindow::~GLFWWindow() {
        auto vkInstance = Renderer::GetInstance();
        vkDestroySurfaceKHR(vkInstance, m_surface, nullptr);

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    bool GLFWWindow::Update() {
        glfwPollEvents();

        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        //ImGui::NewFrame();

        m_layerStack.Draw();

        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //glfwSwapBuffers(m_window);

        return !glfwWindowShouldClose(m_window);
    }

    void GLFWWindow::OnResize(int width, int height) {
        m_width = width;
        m_height = height;
        glfwGetFramebufferSize(m_window, &m_contentWidth, &m_contentHeight);
        m_layerStack.OnEvent(EventResize(m_contentWidth, m_contentHeight));
    }

    void GLFWWindow::OnKey(int key, int scancode, int action, int mods) {
        m_layerStack.OnEvent(EventKey(key, action, mods));
    }

    void GLFWWindow::OnMouseButton(int button, int action, int mods) {
        m_layerStack.OnEvent(EventMouseButton(button, action, mods));
    }

    void GLFWWindow::OnMouseScroll(double xOffset, double yOffset) {
        m_layerStack.OnEvent(EventMouseScroll(xOffset, yOffset));
    }

    void GLFWWindow::OnMouseMove(double x, double y) {
        m_layerStack.OnEvent(EventMouseMove(x, y));
    }

    GLFWWindow::GLFWWindow(GLFWwindow* window, VkSurfaceKHR surface)
        : m_window(window)
        , m_surface(surface)
        , m_layerStack(*this) {
    }
}

#endif
