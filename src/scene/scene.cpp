#include "donut_pch.h"
#include "scene.h"

namespace donut {
    Scene::Scene() {

    }

    Scene::~Scene() {

    }

    Entity Scene::CreateEntity() {
        return Entity(shared_from_this(), m_registry.create());
    }
}