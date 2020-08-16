#pragma once

namespace donut {
    class ICanvas;

    namespace opengl {
        class GLFWWindowImpl : public NonCopyable {
        public:
            static std::shared_ptr<GLFWWindowImpl> Create(int width, int height, std::string const& title);
            ~GLFWWindowImpl();

            bool Update();

            void SetCanvas(std::shared_ptr<ICanvas> canvas) { m_currentCanvas = canvas; }
            std::shared_ptr<ICanvas> GetCanvas() const { return m_currentCanvas; }

            int GetWidth() const { return m_width; }
            int GetHeight() const { return m_height; }

            int GetContentWidth() const { return m_contentWidth; }
            int GetContentHeight() const { return m_contentHeight; }

        private:
            GLFWwindow* m_window = nullptr;
            int m_width = 0, m_height = 0;
            int m_contentWidth = 0, m_contentHeight = 0;

            std::shared_ptr<ICanvas> m_currentCanvas;

            void OnResize(int width, int height);
            void OnKey(int key, int scancode, int action, int mods);
            void OnMouseButton(int button, int action, int mods);
            void OnMouseScroll(double xOffset, double yOffset);
            void OnMouseMove(double x, double y);

            GLFWWindowImpl(GLFWwindow* window);
        };
    }
}
