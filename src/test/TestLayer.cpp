#include "donut_pch.h"
#include "TestLayer.h"

#include "scene/components/transformcomponent.h"
#include "scene/components/cameracomponent.h"
#include "scene/components/meshcomponent.h"
#include "scene/components/inputcomponent.h"
#include "scene/components/orbitcomponent.h"

#include "emittersystem.h"
#include "velocitysystem.h"
#include "lifetimesystem.h"
#include "emittercomponent.h"

#include "FastNoise/FastNoise.h"
#include "FastSIMD/FunctionList.h"

#include "renderer/events/eventdispatch.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <map>

using namespace donut;

Image GetCollapsedNoise() {
    auto const fnSimplex = FastNoise::New<FastNoise::Simplex>();
    auto const fnFractal = FastNoise::New<FastNoise::FractalFBm>();
    auto const gen = FastNoise::New<FastNoise::Remap>();

    fnFractal->SetOctaveCount(5);
    fnFractal->SetSource(fnSimplex);
    gen->SetSource(fnFractal);
    gen->SetRemap(-1.0f, 1.0f, 0.0f, 1.0f);

    int constexpr noiseDim = 700;
    size_t constexpr noiseSize = noiseDim;
    std::vector<float> noise(noiseSize);
    gen->GenUniformGrid2D(noise.data(), 0, 0, noiseDim, 1, 0.002f, 9929);

    std::multimap<float, size_t> visitOrder;
    for (int i = 0; i < noiseSize; ++i)
        visitOrder.insert(std::make_pair(noise[i], i));

    //for (auto&& [value, index] : visitOrder) {
    //    float leftValue = 0;
    //    float rightValue = 0;

    //    if (index > 0) {
    //        leftValue = noise[index - 1];
    //    }

    //    if (index < noiseSize - 1) {
    //        rightValue = noise[index + 1];
    //    }

    //    if (leftValue > value || rightValue > value) {
    //        if (leftValue > rightValue) {
    //            noise[index - 1] += value;
    //        } else {
    //            noise[index + 1] += value;
    //        }
    //        noise[index] = 0;
    //    }
    //}

    std::vector<uint32_t> imageData(noiseSize);
    for (int i = 0; i < noiseSize; ++i) {
        auto const noiseVal = noise[i];
        unsigned char const val = 0x00FF & static_cast<int>(0xFF * noiseVal);
        imageData[i] = 0xFF << 24 | val << 16 | val << 8 | val;
    }

    return Image(noiseDim, 1, ImageFormat::RGBA, imageData.data());
}

Image GetNoisyImage() {
    auto const fnSimplex = FastNoise::New<FastNoise::Simplex>();
    auto const fnFractal = FastNoise::New<FastNoise::FractalFBm>();
    auto const gen = FastNoise::New<FastNoise::ConvertRGBA8>();

    fnFractal->SetOctaveCount(5);
    fnFractal->SetSource(fnSimplex);
    gen->SetSource(fnFractal);

    int constexpr noiseDim = 700;
    size_t constexpr noiseSize = noiseDim * noiseDim;
    std::vector<float> noise(noiseSize);
    gen->GenUniformGrid2D(noise.data(), 0, 0, noiseDim, noiseDim, 0.002f, 9929);

    return Image(noiseDim, noiseDim, ImageFormat::RGBA, noise.data());
}

std::vector<TestVertex> BuildDiscMesh() {
    auto constexpr OutterRadius = 300.0f;
    auto constexpr InnerRadius = 0.0f;
    auto constexpr Segments = 60;

    std::vector<TestVertex> vertices;

    auto angleStep = (2.0f * static_cast<float>(M_PI)) / Segments;
    auto texStep = 1.0f / Segments;
    auto curAngle = 0.0f;
    auto curTex = 0.0f;
    for (int i = 0; i < Segments; ++i) {
        auto const x0 = OutterRadius * sin(curAngle);
        auto const y0 = OutterRadius * cos(curAngle);
        auto const x1 = OutterRadius * sin(curAngle + angleStep);
        auto const y1 = OutterRadius * cos(curAngle + angleStep);
        auto const x2 = InnerRadius * sin(curAngle);
        auto const y2 = InnerRadius * cos(curAngle);
        auto const x3 = InnerRadius * sin(curAngle + angleStep);
        auto const y3 = InnerRadius * cos(curAngle + angleStep);

        vertices.push_back({ { x0, y0, 0 }, { 1, curTex } });
        vertices.push_back({ { x2, y2, 0 }, { 0, curTex } });
        vertices.push_back({ { x1, y1, 0 }, { 1, curTex + texStep } });

        vertices.push_back({ { x1, y1, 0 }, { 1, curTex + texStep } });
        vertices.push_back({ { x2, y2, 0 }, { 0, curTex } });
        vertices.push_back({ { x3, y3, 0 }, { 0, curTex + texStep } });

        curAngle += angleStep;
        curTex += texStep;
    }

    return vertices;
}

std::vector<TestVertex> BuildQuadMesh() {
    auto constexpr Size = 100.0f;
    auto constexpr HalfSize = Size / 2.0f;
    return {
        { { -HalfSize, HalfSize, 0 }, { 0, 0 } },
        { { -HalfSize, -HalfSize, 0 }, { 0, 1 } },
        { { HalfSize, HalfSize, 0 }, { 1, 0 } },
        { { HalfSize, HalfSize, 0 }, { 1, 0 } },
        { { -HalfSize, -HalfSize, 0 }, { 0, 1 } },
        { { HalfSize, -HalfSize, 0 }, { 1, 1 } }
    };
}

MeshComponent BuildGridMesh(int width, int height, float sizeX, float sizeY) {
    std::vector<TestVertex> vertices;

    auto const horizLines = width + 1;
    auto const vertLines = height + 1;
    auto const horizSpacing = sizeX / width;
    auto const vertSpacing = sizeY / height;

    for (int i = 0; i < horizLines; ++i) {
        auto const x1 = -sizeX / 2.0f;
        auto const y1 = -sizeY / 2.0f + i * horizSpacing;
        auto const x2 = sizeX / 2.0f;
        auto const y2 = y1;
        vertices.push_back({ { x1, y1, 0 }, { 0, 0 } });
        vertices.push_back({ { x2, y2, 0 }, { 0, 0 } });
    }

    for (int i = 0; i < vertLines; ++i) {
        auto const x1 = -sizeX / 2.0f + i * vertSpacing;
        auto const y1 = -sizeY / 2.0f;
        auto const x2 = x1;
        auto const y2 = sizeY / 2.0f;
        vertices.push_back({ { x1, y1, 0 }, { 0, 0 } });
        vertices.push_back({ { x2, y2, 0 }, { 0, 0 } });
    }

    std::vector<unsigned int> indices(vertices.size());
    for (int i = 0; i < vertices.size(); ++i)
        indices[i] = i;

    VertexLayout layout {
        VertexElement { VertexElementType::Float, 3, false },
        VertexElement { VertexElementType::Float, 2, false }
    };

    MeshComponent mesh;
    mesh.m_primitiveType = PrimitiveType::LINES;
    mesh.m_vertexBuffer = VertexBuffer::Create(layout, vertices.data(), static_cast<unsigned int>(vertices.size()));
    mesh.m_indexBuffer = IndexBuffer::Create(indices.data(), static_cast<unsigned int>(indices.size()));
    return mesh;
}

TestLayer::TestLayer() {
    m_scene = std::make_shared<donut::Scene>();
    m_emitterSystem = std::make_unique<EmitterSystem>();
    m_velocitySystem = std::make_unique<VelocitySystem>();
    m_lifetimeSystem = std::make_unique<LifetimeSystem>();
    m_cameraSystem = std::make_unique<CameraSystem>();
    m_inputSystem = std::make_unique<InputSystem>(*m_scene);
    m_renderingSystem = std::make_unique<RenderingSystem>();

    auto emitter = m_scene->CreateEntity();
    auto& emitterParams = emitter.AddComponent<EmitterComponent>();
    emitterParams.m_spawnDelay = 800;
    emitterParams.m_lifetime = 5000;
    emitterParams.m_initialVelocity.m_linearVelocity = { 0.0f, 0.0f, -40.0f };
    emitterParams.m_initialVelocity.m_angularVelocity = { 0.0f, 0.0f, 3.0f };

    VertexLayout layout {
        VertexElement { VertexElementType::Float, 3, false },
        VertexElement { VertexElementType::Float, 2, false }
    };

    //auto verts = BuildDiscMesh();
    auto verts = BuildQuadMesh();
    std::vector<unsigned int> indices(verts.size());
    for (int i = 0; i < verts.size(); ++i)
        indices[i] = i;

    std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(layout, verts.data(), static_cast<unsigned int>(verts.size()));
    std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(indices.data(), static_cast<unsigned int>(indices.size()));
    emitterParams.m_mesh.m_vertexBuffer = vb;
    emitterParams.m_mesh.m_indexBuffer = ib;

    FrameBufferDesc frameBufferDesc;
    frameBufferDesc.m_width = 100;
    frameBufferDesc.m_height = 100;
    FrameBufferTextureDesc colorDesc;
    colorDesc.m_format = FrameBufferTextureFormat::RGBA;
    frameBufferDesc.m_attachments.push_back(colorDesc);
    m_frameBuffer = FrameBuffer::Create(frameBufferDesc);

    m_frameBuffer->Bind();
    Renderer::Clear({ 1.0f, 1.0f, 0.0f, 0.3f });
    m_frameBuffer->Unbind();

    //Image img = GetCollapsedNoise();
    Image img = GetNoisyImage();
    emitterParams.m_mesh.m_texture = Texture2D::Create(img, TextureFormat::RGBA);
    //emitterParams.m_mesh.m_texture = m_frameBuffer->GetColorTexture(0);

    auto gridEntity = m_scene->CreateEntity();
    gridEntity.AddComponent<MeshComponent>(BuildGridMesh(5, 5, 10.0f, 10.f));
    auto& gridTransform = gridEntity.AddComponent<TransformComponent>();
    gridTransform.m_transform = glm::translate(glm::identity<glm::mat4x4>(), { 0.0f, 0.0f, 0.0f });
}

TestLayer::~TestLayer() {

}

void TestLayer::OnAddedToStack(LayerStack& stack) {
    auto& window = stack.GetWindow();
    m_width = window.GetContentWidth();
    m_height = window.GetContentHeight();

    auto camera = m_scene->CreateEntity();
    auto& cameraTransformComponent = camera.AddComponent<TransformComponent>();
    auto& cameraComponent = camera.AddComponent<CameraComponent>();
    auto& projectionComponent = camera.AddComponent<PerspectiveProjectionComponent>();
    auto& inputComponent = camera.AddComponent<InputComponent>();
    auto& orbitComponent = camera.AddComponent<OrbitComponent>();

    inputComponent.m_enabled = true;
    orbitComponent.m_target = glm::vec3(0, 0, 0);
    orbitComponent.m_length = 80.0f;
    orbitComponent.m_yawScale = 0.01f;
    orbitComponent.m_pitchScale = 0.01f;

    cameraTransformComponent.m_transform = glm::translate(glm::identity<glm::mat4x4>(), { 0.0f, 0.0f, -80.0f });
    cameraComponent.m_active = true;
    projectionComponent.m_fov = glm::radians(70.0f);
    projectionComponent.m_aspect = m_width / static_cast<float>(m_height);
    projectionComponent.m_near = 0.1f;
    projectionComponent.m_far = 1000.0f;

    m_lastTime = std::chrono::system_clock::now();
}

void TestLayer::OnRemovedFromStack(LayerStack& stack) {

}

bool TestLayer::OnEvent(Event const& event) {
    EventDispatch dispatch(event);
    dispatch.Dispatch(m_inputSystem.get());
    dispatch.Dispatch(this, &TestLayer::OnResizeEvent);
    dispatch.Dispatch(this, &TestLayer::OnKeyEvent);
    dispatch.Dispatch(this, &TestLayer::OnMouseButtonEvent);
    dispatch.Dispatch(this, &TestLayer::OnMouseScrollEvent);
    dispatch.Dispatch(this, &TestLayer::OnMouseMoveEvent);
    return dispatch.WasDispatched();
}

void TestLayer::Draw() {
    auto time = std::chrono::system_clock::now();
    auto deltaTime = time - m_lastTime;
    auto const timestep = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count());
    m_lastTime = time;
    Renderer::Viewport(0, 0, m_width, m_height);
    //m_emitterSystem->Update(*m_scene, timestep);
    //m_velocitySystem->Update(*m_scene, timestep);
    //m_lifetimeSystem->Update(*m_scene, timestep);
    m_cameraSystem->Update(*m_scene);
    m_renderingSystem->Update(*m_scene);

    if (ImGui::Begin("Entities")) {
        auto& registry = m_scene->GetRegistry();
        registry.each([&](auto entity) {
            if (ImGui::CollapsingHeader("Entity")) {
                OrbitComponent::ImGuiComponent(registry, entity);
            }
        });
        ImGui::End();
    }
    ImGui::Text("Hello world");
}

bool TestLayer::OnResizeEvent(EventResize const& event) {
    m_width = event.GetWidth();
    m_height = event.GetHeight();
    return false;
}

bool TestLayer::OnKeyEvent(EventKey const& event) {
    return false;
}

bool TestLayer::OnMouseButtonEvent(EventMouseButton const& event) {
    return false;
}

bool TestLayer::OnMouseScrollEvent(EventMouseScroll const& event) {
    return false;
}

bool TestLayer::OnMouseMoveEvent(EventMouseMove const& event) {
    return false;
}
