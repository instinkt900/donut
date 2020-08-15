#pragma once

namespace donut {
    class ICanvas;

	class Window : public NonCopyable {
    public:
        Window(int width, int height, std::string const& title);
        ~Window();

        void Update();

        bool Valid() const;
        operator bool() const { return Valid(); }

        void SetCanvas(std::shared_ptr<ICanvas> canvas);

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        bool m_initialised = false;
        GLFWwindow* m_window = nullptr;
        int m_width = 0, m_height = 0;

        std::shared_ptr<ICanvas> m_currentCanvas;

        void OnResize(int width, int height);
        void OnKey(int key, int scancode, int action, int mods);
        void OnMouseButton(int button, int action, int mods);
        void OnMouseScroll(double xOffset, double yOffset);
        void OnMouseMove(double x, double y);
	};
}