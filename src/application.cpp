#include "donut_pch.h"
#include "application.h"
#include "renderer/window.h"

namespace donut {
    std::shared_ptr<Application> Application::s_instance = nullptr;

    std::shared_ptr<Application> Application::Create(int windowWidth, int windowHeight, std::string const& name) {
        if (s_instance) {
            spdlog::warn("Application already created!");
        } else {
            s_instance.reset(new Application(windowWidth, windowHeight, name));
        }
        return s_instance;

    }

    Application::Application(int windowWidth, int windowHeight, std::string const& name)
        : m_name(name) {
        assert(s_instance == nullptr && "Application already created.");
        m_window = Window::Create(windowWidth, windowHeight, m_name);
    }

    Application::~Application() {
        s_instance = nullptr;
    }

    void Application::Run() {
        Renderer::Init();
        bool windowOpen = true;
        while (windowOpen) {
            windowOpen = m_window->Update();
        }
        Renderer::Shutdown();
    }
}
