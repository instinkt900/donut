#include "donut_pch.h"
#include "opengl_glfw_window.h"

#include "renderer/events/eventresize.h"
#include "renderer/events/eventkey.h"
#include "renderer/events/eventmousebutton.h"
#include "renderer/events/eventmousescroll.h"
#include "renderer/events/eventmousemove.h"

#define GET_WINDOW(glfwWindow) static_cast<GLFWWindow*>(glfwGetWindowUserPointer(glfwWindow))

namespace donut::opengl {
    std::shared_ptr<GLFWWindow> GLFWWindow::Create(int width, int height, std::string const& title) {
        if (!glfwInit()) {
            spdlog::error("glfwInit failed!");
            return nullptr;
        }

        glfwSetErrorCallback([](int error, char const* desc) {
            spdlog::error("glfw error: {}", desc);
        });

        auto window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (nullptr == window) {
            spdlog::error("glfwCreateWindow failed!");
            return nullptr;
        }

        return std::shared_ptr<GLFWWindow>(new GLFWWindow(window));
    }

    GLFWWindow::~GLFWWindow() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    bool GLFWWindow::Update() {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_layerStack.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);

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

    GLFWWindow::GLFWWindow(GLFWwindow* window)
        : m_window(window)
        , m_layerStack(*this) {
        glfwGetWindowSize(m_window, &m_width, &m_height);
        glfwGetFramebufferSize(m_window, &m_contentWidth, &m_contentHeight);
        glfwSetWindowUserPointer(m_window, this);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* glfwWindow, int width, int height) {
            GET_WINDOW(glfwWindow)->OnResize(width, height);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
            GET_WINDOW(glfwWindow)->OnKey(key, scancode, action, mods);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* glfwWindow, int button, int action, int mods) {
            GET_WINDOW(glfwWindow)->OnMouseButton(button, action, mods);
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* glfwWindow, double xOffset, double yOffset) {
            GET_WINDOW(glfwWindow)->OnMouseScroll(xOffset, yOffset);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* glfwWindow, double x, double y) {
            GET_WINDOW(glfwWindow)->OnMouseMove(x, y);
        });

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            spdlog::error("gladLoadGLLoader failed");
            return;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init();

        spdlog::info("OpenGL Info:");
        spdlog::info("  Vendor: {0}", glGetString(GL_VENDOR));
        spdlog::info("  Renderer: {0}", glGetString(GL_RENDERER));
        spdlog::info("  Version: {0}", glGetString(GL_VERSION));
    }
}