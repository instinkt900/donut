#include "donut_pch.h"
#include "testcanvas.h"

#include "FastNoise/FastNoise.h"

#include <map>

void TestNoise() {
    auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
    auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

    fnFractal->SetSource(fnSimplex);
    fnFractal->SetOctaveCount(5);

    size_t constexpr noiseSize = 700;
    std::vector<float> noise(noiseSize);
    fnFractal->GenUniformGrid2D(noise.data(), 0, 128, noiseSize, 1, 0.002f, 9929);

    for (auto&& val : noise) {
        val = (val + 1.0f) / 2.0f;
    }

    std::multimap<float, size_t> visitOrder;
    for (int i = 0; i < noiseSize; ++i)
        visitOrder.insert(std::make_pair(noise[i], i));

    for (auto&& [value, index] : visitOrder) {
        
        float leftValue = 0;
        float rightValue = 0;

        if (index > 0) { 
            leftValue = noise[index - 1];
        }

        if (index < noiseSize - 1) {
            rightValue = noise[index + 1];
        }

        if (leftValue > value || rightValue > value) {
            if (leftValue > rightValue) {
                noise[index - 1] += value;
            } else {
                noise[index + 1] += value;
            }
            noise[index] = 0;
        }
    }

    return;
}

int main(int argc, char const** argv) {
    TestNoise();
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
