#include "donut_pch.h"
#include "testcanvas.h"

int main(int argc, char const** argv) {
    donut::Window window(1280, 720, "Test Window");
    if (window) {
        std::shared_ptr<donut::ICanvas> scene = std::make_shared<TestCanvas>();
        window.SetCanvas(scene);
        bool windowOpen = true;
        while (windowOpen) {
            windowOpen = window.Update();
        }
    }
    return 0;
}
