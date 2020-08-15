#include "donut_pch.h"
#include "window.h"
#include "testcanvas.h"

int main(int argc, char const** argv) {
    donut::Window window(1280, 720, "poo");
    std::shared_ptr<donut::ICanvas> scene = std::make_shared<donut::TestCanvas>();
    window.SetCanvas(scene);
    while (window) {
        window.Update();
    }
    return 0;
}
