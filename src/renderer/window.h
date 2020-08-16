#pragma once

#include "render_impl/window_impl.h"

namespace donut {
	class Window {
    public:
        Window() = default;
        Window(int width, int height, std::string const& title);
        Window(Window const&) = default;
        Window& operator=(Window const&) = default;
        ~Window() = default;

        bool Update();

        bool Valid() const { return m_impl != nullptr; }
        operator bool() const { return Valid(); }

        void SetCanvas(std::shared_ptr<ICanvas> canvas);

        int GetWidth() const;
        int GetHeight() const;

        int GetContentWidth() const;
        int GetContentHeight() const;

    private:
        std::shared_ptr<WindowImpl> m_impl;
	};
}