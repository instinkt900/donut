#include "donut_pch.h"
#include "testcanvas.h"

#include "scene/components/transformcomponent.h"
#include "scene/components/cameracomponent.h"
#include "scene/components/meshcomponent.h"

#include "emittersystem.h"
#include "velocitysystem.h"
#include "lifetimesystem.h"
#include "emittercomponent.h"

#include "FastNoise/FastNoise.h"
#include "FastSIMD/FunctionList.h"

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

TestCanvas::TestCanvas() {
    m_scene = std::make_shared<donut::Scene>();
    m_emitterSystem = std::make_unique<EmitterSystem>();
    m_velocitySystem = std::make_unique<VelocitySystem>();
    m_lifetimeSystem = std::make_unique<LifetimeSystem>();
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
}

TestCanvas::~TestCanvas() {

}

void TestCanvas::OnAddedToWindow(Window* window) {
    m_width = window->GetContentWidth();
    m_height = window->GetContentHeight();

    auto camera = m_scene->CreateEntity();
    auto& cameraTransformComponent = camera.AddComponent<TransformComponent>();
    auto& cameraComponent = camera.AddComponent<CameraComponent>();

    cameraTransformComponent.m_transform = glm::translate(glm::identity<glm::mat4x4>(), { 0.0f, 0.0f, -80.0f });
    cameraComponent.m_active = true;
    cameraComponent.m_fov = glm::radians(70.0f);
    cameraComponent.m_aspect = m_width / static_cast<float>(m_height);
    cameraComponent.m_near = 0.1f;
    cameraComponent.m_far = 1000.0f;

    m_lastTime = std::chrono::system_clock::now();
}

void TestCanvas::OnRemovedFromWindow(Window* window) {

}

void TestCanvas::OnResize(int width, int height) {
    m_width = width;
    m_height = height;
}

void TestCanvas::OnKey(int key, int action, int mods) {

}

void TestCanvas::OnMouseButton(int button, int action, int mods) {

}

void TestCanvas::OnMouseScroll(double xOffset, double yOffset) {

}

void TestCanvas::OnMouseMove(double x, double y) {

}

void TestCanvas::Draw() {
    auto time = std::chrono::system_clock::now();
    auto deltaTime = time - m_lastTime;
    auto const timestep = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count());
    m_lastTime = time;
    Renderer::Viewport(0, 0, m_width, m_height);
    m_emitterSystem->Update(*m_scene, timestep);
    m_velocitySystem->Update(*m_scene, timestep);
    m_lifetimeSystem->Update(*m_scene, timestep);
    m_renderingSystem->Update(*m_scene);
    ImGui::Text("Hello world");
}
