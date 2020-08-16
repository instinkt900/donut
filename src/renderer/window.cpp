#include "donut_pch.h"
#include "window.h"

#include "icanvas.h"

namespace donut {
	Window::Window(int width, int height, std::string const& title) {
        m_impl = WindowImpl::Create(width, height, title);
	}

	bool Window::Update() {
        assert(Valid());
        return m_impl->Update();
	}

	void Window::SetCanvas(std::shared_ptr<ICanvas> canvas) {
        auto currentCanvas = m_impl->GetCanvas();
        if (nullptr != currentCanvas) {
            currentCanvas->OnRemovedFromWindow(*this);
		}

        m_impl->SetCanvas(canvas);

		currentCanvas = m_impl->GetCanvas();
		if (nullptr != currentCanvas) {
            currentCanvas->OnAddedToWindow(*this);
		}
    }

	int Window::GetWidth() const {
        assert(Valid());
        return m_impl->GetWidth();
	}

	int Window::GetHeight() const {
        assert(Valid());
        return m_impl->GetHeight();
	}

	int Window::GetContentWidth() const {
        assert(Valid());
        return m_impl->GetContentWidth();
	}

	int Window::GetContentHeight() const {
        assert(Valid());
        return m_impl->GetContentHeight();
	}
}
