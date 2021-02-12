#pragma once

#include "scene/components/meshcomponent.h"
#include "velocitycomponent.h"

namespace donut {
    struct EmitterComponent {
        int m_spawnDelay;
        MeshComponent m_mesh;
        VelocityComponent m_initialVelocity;
        int m_lifetime;

        int m_spawnTimer = 0;
    };
}
