#pragma once

#include "scene/scene.h"

namespace donut {
    class CameraSystem {
    public:
        CameraSystem() = default;
        ~CameraSystem() = default;

        void Update(Scene& scene);

    private:
    };
}
