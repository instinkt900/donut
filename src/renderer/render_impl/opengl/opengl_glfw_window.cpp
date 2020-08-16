#include "donut_pch.h"
#include "opengl_glfw_window.h"

#define GET_WINDOW(glfwWindow) static_cast<GLFWWindowImpl*>(glfwGetWindowUserPointer(glfwWindow))

namespace donut::opengl {
    std::shared_ptr<GLFWWindowImpl> GLFWWindowImpl::Create(int width, int height, std::string const& title) {
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

        return std::shared_ptr<GLFWWindowImpl>(new GLFWWindowImpl(window));
    }

    GLFWWindowImpl::~GLFWWindowImpl() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    bool GLFWWindowImpl::Update() {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (nullptr != m_currentCanvas) {
            m_currentCanvas->Draw();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);

        return !glfwWindowShouldClose(m_window);
    }

    void GLFWWindowImpl::OnResize(int width, int height) {
        m_width = width;
        m_height = height;
        glfwGetFramebufferSize(m_window, &m_contentWidth, &m_contentHeight);
        if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnResize(m_contentWidth, m_contentHeight);
        }
    }

    void GLFWWindowImpl::OnKey(int key, int scancode, int action, int mods) {
        if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnKey(key, action, mods);
        }
    }

    void GLFWWindowImpl::OnMouseButton(int button, int action, int mods) {
        if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnMouseButton(button, action, mods);
        }
    }

    void GLFWWindowImpl::OnMouseScroll(double xOffset, double yOffset) {
        if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnMouseScroll(xOffset, yOffset);
        }
    }

    void GLFWWindowImpl::OnMouseMove(double x, double y) {
        if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnMouseMove(x, y);
        }
    }

    GLFWWindowImpl::GLFWWindowImpl(GLFWwindow* window)
        : m_window(window) {
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
    }
}