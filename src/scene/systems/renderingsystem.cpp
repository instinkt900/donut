#include "donut_pch.h"
#include "renderingsystem.h"

#include "scene/components/transformcomponent.h"
#include "scene/components/cameracomponent.h"
#include "scene/components/meshcomponent.h"

namespace {
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
    //fragColor.a = 1.0f;
}
)";
}

namespace donut {
    RenderingSystem::RenderingSystem() {
        auto const vertexSection = ShaderSection::Create(ShaderSectionType::Vertex, VertexShaderSource);
        auto const fragmentSection = ShaderSection::Create(ShaderSectionType::Fragment, FragmentShaderSource);
        m_shader = Shader::Create({ vertexSection.get(), fragmentSection.get() });
    }

    void RenderingSystem::Update(Scene& scene) {
        Renderer::Clear({ 0, 0, 0, 1 });
        m_shader->Bind();

        scene.GetRegistry().view<CameraComponent>().each([&](entt::entity entity, CameraComponent const& cameraComponent) {
            if (cameraComponent.m_active) {
                m_shader->SetMatrix44("viewProjection", cameraComponent.m_projection * cameraComponent.m_view);
            }
        });

        scene.GetRegistry().view<TransformComponent, MeshComponent>().each([&](entt::entity entity, TransformComponent const& transformComponent, MeshComponent const& meshComponent) {
            m_shader->SetMatrix44("model", transformComponent.m_transform);
            if (meshComponent.m_texture)
                meshComponent.m_texture->Bind();
            meshComponent.m_vertexBuffer->Bind();
            meshComponent.m_indexBuffer->Bind();
            Renderer::DrawIndexedPrimitives(meshComponent.m_primitiveType, meshComponent.m_indexBuffer->GetCount());
            meshComponent.m_indexBuffer->Unbind();
            meshComponent.m_vertexBuffer->Unbind();
            if (meshComponent.m_texture)
                meshComponent.m_texture->Unbind();
        });

        m_shader->Unbind();
    }
}
