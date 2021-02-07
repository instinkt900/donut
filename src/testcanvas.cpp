#include "donut_pch.h"
#include "testcanvas.h"

#define _USE_MATH_DEFINES
#include <math.h>

auto const VertexShaderSource = R"(
#version 330 core
uniform mat4 viewProjection;
uniform mat4 model;
layout (location = 0) in vec4 inVertexPosition;
layout (location = 1) in vec2 inVertexUV;
out vec2 vertexUV;
void main()
{
    gl_Position = viewProjection * model * vec4(inVertexPosition.xyz, 1.0);
    vertexUV = inVertexUV;
}
)";

auto const FragmentShaderSource = R"(
#version 330 core
uniform vec4 color;
uniform sampler2D textureSampler;
in vec2 vertexUV;
out vec4 fragColor;

void main()
{
    fragColor = texture(textureSampler, vertexUV);
    //fragColor.rg = vertexUV;
    //fragColor.b = 0.0f;
    fragColor.a = 1.0f;
}
)";

using namespace donut;

#include "FastNoise/FastNoise.h"
#include "FastSIMD/FunctionList.h"

#include <map>

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
    m_shader = Shader { ShaderSection(ShaderSectionType::Vertex, VertexShaderSource),
        ShaderSection(ShaderSectionType::Fragment, FragmentShaderSource) };

    VertexLayout layout {
        VertexElement { VertexElementType::Float, 3, false },
        VertexElement { VertexElementType::Float, 2, false }
    };

    //auto verts = BuildDiscMesh();
    auto verts = BuildQuadMesh();
    std::vector<unsigned int> indices(verts.size());
    for (int i = 0; i < verts.size(); ++i)
        indices[i] = i;

    VertexBuffer vb(layout, verts.data(), verts.size());
    IndexBuffer ib(indices.data(), indices.size());
    m_mesh = Mesh(vb, ib);

    //Image img = GetCollapsedNoise();
    Image img = GetNoisyImage();

    m_texture = Texture2D(img);

    auto const ortho = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f);
    m_shader.Bind();
    m_shader.SetMatrix44("viewProjection", ortho);
    m_shader.SetMatrix44("model", glm::mat4x4(1.0f));
    m_shader.SetVector4("color", glm::vec4(1, 1, 1, 1));
    m_shader.Unbind();
}

TestCanvas::~TestCanvas() {

}

void TestCanvas::OnAddedToWindow(Window window) {
    m_width = window.GetContentWidth();
    m_height = window.GetContentHeight();
}

void TestCanvas::OnRemovedFromWindow(Window window) {

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
    glViewport(0, 0, m_width, m_height);

    m_shader.Bind();
    m_texture.Bind();
    m_mesh.Draw();
    m_texture.Unbind();
    m_shader.Unbind();

    ImGui::Text("Hello world");
}
