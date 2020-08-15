#include "donut_pch.h"
#include "window.h"

#include "icanvas.h"

#define GET_WINDOW(glfwWindow) static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))

namespace donut {
	Window::Window(int width, int height, std::string const& title) {
		if (!glfwInit()) {
            spdlog::error("glfwInit failed!");
            return;
		}

		glfwSetErrorCallback([](int error, char const* desc) {
            spdlog::error("glfw error: {}", desc);
        });

		m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (nullptr == m_window) {
            spdlog::error("glfwCreateWindow failed!");
            return;
		}

		glfwGetWindowSize(m_window, &m_width, &m_height);
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

		m_initialised = true;
	}

	Window::~Window() {
		if (m_initialised) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
		}

        if (nullptr != m_window)
            glfwDestroyWindow(m_window);

        if (m_initialised)
            glfwTerminate();
	}

	void Window::Update() {
        if (!Valid())
            return;

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
	}

	bool Window::Valid() const {
        return m_initialised && nullptr != m_window && !glfwWindowShouldClose(m_window);
	}

	void Window::SetCanvas(std::shared_ptr<ICanvas> canvas) {
		if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnRemovedFromWindow(this);
		}
        m_currentCanvas = canvas;
		if (nullptr != m_currentCanvas) {
            int fbWidth, fbHeight;
            glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
            m_currentCanvas->OnAddedToWindow(this, fbWidth, fbHeight);
		}
    }

	void Window::OnResize(int width, int height) {
        m_width = width;
        m_height = height;

		if (nullptr != m_currentCanvas) {
            int fbWidth, fbHeight;
            glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
            m_currentCanvas->OnResize(fbWidth, fbHeight);
		}
	}

	void Window::OnKey(int key, int scancode, int action, int mods) {
		if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnKey(key, action, mods);
		}
	}

	void Window::OnMouseButton(int button, int action, int mods) {
		if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnMouseButton(button, action, mods);
		}
	}

	void Window::OnMouseScroll(double xOffset, double yOffset) {
		if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnMouseScroll(xOffset, yOffset);
		}
	}

	void Window::OnMouseMove(double x, double y) {
		if (nullptr != m_currentCanvas) {
            m_currentCanvas->OnMouseMove(x, y);
		}
	}
}
