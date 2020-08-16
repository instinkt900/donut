#include "donut_pch.h"
#include "testcanvas.h"

auto const VertexShaderSource = R"(
#version 330 core
uniform mat4 viewProjection;
uniform mat4 model;
layout (location = 0) in vec4 inVertexPosition;
layout (location = 1) in vec4 inVertexColor;
out vec4 vertexColor;
void main()
{
    gl_Position = viewProjection * model * vec4(inVertexPosition.xyz, 1.0);
    vertexColor = inVertexColor;
}
)";

auto const FragmentShaderSource = R"(
#version 330 core
uniform vec4 color;
in vec4 vertexColor;
out vec4 fragColor;
void main()
{
    fragColor = vertexColor * color;
}
)";

using namespace donut;

TestCanvas::TestCanvas() {
    m_shader = Shader { ShaderSection(ShaderSectionType::Vertex, VertexShaderSource),
        ShaderSection(ShaderSectionType::Fragment, FragmentShaderSource) };

    VertexLayout layout {
        VertexElement { VertexElementType::Float, 3, false },
        VertexElement { VertexElementType::Float, 4, false }
    };

    TestVertex verts[] = {
        { { -0.5, -0.5, 0 }, { 1, 1, 1, 1 } },
        { { 0.5, -0.5, 0 }, { 1, 1, 1, 1 } },
        { { 0, 0.5, 0 }, { 1, 1, 1, 1 } },
    };

    unsigned int indices[] { 0, 1, 2 };

    VertexBuffer vb(layout, verts, 3);
    IndexBuffer ib(indices, 3);
    m_mesh = Mesh(vb, ib);

    auto const ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
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
    m_mesh.Draw();
    m_shader.Unbind();

    ImGui::Text("Hello world");
}
