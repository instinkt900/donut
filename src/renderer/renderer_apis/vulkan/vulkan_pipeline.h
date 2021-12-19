#pragma once

namespace donut::vulkan {
    class ShaderProgram;
    class RenderPass;
    class VertexLayout;
    class RenderMode;
    class Context;

    class PipelineInitializer {
    public:
        PipelineInitializer(ShaderProgram const* shader, RenderPass const* renderPass, VertexLayout const* vertexLayout, RenderMode const* renderMode);

        ShaderProgram const* const m_shader;
        RenderPass const* const m_renderPass;
        VertexLayout const* const m_vertexLayout;
        RenderMode const* const m_renderMode;

    private:
        uint32_t const m_hash;
    };

    class Pipeline {
    public:
        Pipeline(Context* context, PipelineInitializer const& initializer);

        VkPipeline GetPipeline() const { return m_pipeline; }
        VkPipelineLayout GetLayout() const { return m_layout; }

    private:
        VkPipeline m_pipeline;
        VkPipelineLayout m_layout;
    };
}
