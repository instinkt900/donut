#pragma once

#include "iwindow.h"
#include "layerstack.h"

namespace donut {
    // TODO this is a bad name. GLFWWindow vs GLFWwindow
    class GLFWWindow : public IWindow, public NonCopyable {
    public:
        static std::shared_ptr<GLFWWindow> Create(int width, int height, std::string const& title);
        virtual ~GLFWWindow();

        bool Update() override;

        LayerStack& GetLayerStack() override { return m_layerStack; }

        int GetWidth() const override { return m_width; }
        int GetHeight() const override { return m_height; }

        int GetContentWidth() const override { return m_contentWidth; }
        int GetContentHeight() const override { return m_contentHeight; }

        GLFWwindow* GetGLFWWindow() const { return m_window; }

    private:
        GLFWwindow* m_window = nullptr;
        int m_width = 0, m_height = 0;
        int m_contentWidth = 0, m_contentHeight = 0;

        LayerStack m_layerStack;

        void OnResize(int width, int height);
        void OnKey(int key, int scancode, int action, int mods);
        void OnMouseButton(int button, int action, int mods);
        void OnMouseScroll(double xOffset, double yOffset);
        void OnMouseMove(double x, double y);

        GLFWWindow(GLFWwindow* window);
    };
}
