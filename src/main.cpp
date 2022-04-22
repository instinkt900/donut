#include "donut_pch.h"
#include "test/TestLayer.h"

int main(int argc, char const** argv) {
    auto window = donut::Window::Create(1280, 720, "Test Window");
    if (window) {
        donut::Renderer::Init();
        auto layer = std::make_shared<donut::TestLayer>();
        window->GetLayerStack().PushLayer(layer);
        bool windowOpen = true;
        while (windowOpen) {
            windowOpen = window->Update();
        }
    }
    return 0;
}
