#include "donut_pch.h"
#include "vulkan_pipeline.h"

#include "vulkan_rendermode.h"
#include "vulkan_utils.h"
#include "vulkan_shader.h"
#include "vulkan_renderpass.h"
#include "vulkan_vertexlayout.h"
#include "vulkan_context.h"
#include "vulkan_reflectioninfo.h"

namespace {
    using namespace donut::vulkan;

    VkBool32 ToVulkan(bool value) { return value ? VK_TRUE : VK_FALSE; }
    VkPolygonMode ToVulkan(FillMode value) {
        switch (value) {
            default:
            case FillMode::Fill: return VK_POLYGON_MODE_FILL;
            case FillMode::Line: return VK_POLYGON_MODE_LINE;
            case FillMode::Point: return VK_POLYGON_MODE_POINT;
        }
    }
    VkCullModeFlags ToVulkan(CullMode value) {
        switch (value) {
            default:
            case CullMode::Back: return VK_CULL_MODE_BACK_BIT;
            case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
        }
    }
    VkCompareOp ToVulkan(CompareOp value) {
        switch (value) {
            default:
            case CompareOp::Always: return VK_COMPARE_OP_ALWAYS;
            case CompareOp::Equal: return VK_COMPARE_OP_EQUAL;
            case CompareOp::Greater: return VK_COMPARE_OP_GREATER;
            case CompareOp::GreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
            case CompareOp::Less: return VK_COMPARE_OP_LESS;
            case CompareOp::LessOrEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
            case CompareOp::Never: return VK_COMPARE_OP_NEVER;
            case CompareOp::NotEqual: return VK_COMPARE_OP_NOT_EQUAL;
        }
    }
    VkBlendFactor ToVulkan(BlendFactor value) {
        switch (value) {
            default:
            case BlendFactor::ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
            case BlendFactor::ConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
            case BlendFactor::DstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
            case BlendFactor::DstColor: return VK_BLEND_FACTOR_DST_COLOR;
            case BlendFactor::One: return VK_BLEND_FACTOR_ONE;
            case BlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
            case BlendFactor::OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
            case BlendFactor::OneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
            case BlendFactor::OneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            case BlendFactor::OneMinusSrc1Alpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
            case BlendFactor::OneMinusSrc1Color: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
            case BlendFactor::OneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::OneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            case BlendFactor::Src1Alpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
            case BlendFactor::Src1Color: return VK_BLEND_FACTOR_SRC1_COLOR;
            case BlendFactor::SrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
            case BlendFactor::SrcAlphaSaturate: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
            case BlendFactor::SrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
            case BlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
        }
    }
    VkBlendOp ToVulkan(BlendOp value) {
        switch (value) {
            default:
            case BlendOp::Add: return VK_BLEND_OP_ADD;
            case BlendOp::Max: return VK_BLEND_OP_MAX;
            case BlendOp::Min: return VK_BLEND_OP_MIN;
            case BlendOp::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
            case BlendOp::Subtract: return VK_BLEND_OP_SUBTRACT;
        }
    }
}

namespace donut::vulkan {
    PipelineInitializer::PipelineInitializer(ShaderProgram const* shader, RenderPass const* renderPass, VertexLayout const* vertexLayout, RenderMode const* renderMode)
        : m_shader(shader)
        , m_renderPass(renderPass)
        , m_vertexLayout(vertexLayout)
        , m_renderMode(renderMode)
        , m_hash(Hash(std::initializer_list{ m_shader->GetHash(), m_renderPass->GetHash(), m_vertexLayout->GetHash(), m_renderMode->GetHash() }))
    {
       
    }

    Pipeline::Pipeline(Context* context, PipelineInitializer const& initializer) {

        std::array<VkPipelineShaderStageCreateInfo,5> shaderStages;
        int shaderStageCount = 0;

        if (initializer.m_shader->VertexModule()) {
            VkPipelineShaderStageCreateInfo vertShaderStageInfo {};
            vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            vertShaderStageInfo.module = initializer.m_shader->VertexModule()->GetModule();
            vertShaderStageInfo.pName = initializer.m_shader->GetEntryPoint(ShaderProgram::Stage::Vertex);
            shaderStages[shaderStageCount] = vertShaderStageInfo;
            ++shaderStageCount;
        }

        if (initializer.m_shader->FragmentModule()) {
            VkPipelineShaderStageCreateInfo fragShaderStageInfo {};
            fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            fragShaderStageInfo.module = initializer.m_shader->FragmentModule()->GetModule();
            fragShaderStageInfo.pName = initializer.m_shader->GetEntryPoint(ShaderProgram::Stage::Fragment);
            shaderStages[shaderStageCount] = fragShaderStageInfo;
            ++shaderStageCount;
        }

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        for (auto& set : initializer.m_shader->GetDescriptorSets()) {
            if (set.IsEmpty())
                continue;
            descriptorSetLayouts.push_back(set.layout);
        }

		auto& pushConstants = initializer.m_shader->GetPushConstants();
        std::vector<VkPushConstantRange> pushConstantRanges;
        for (int i = 0; i < static_cast<int>(pushConstants.size()); i++) {
            auto& item = pushConstants[i];
            VkPushConstantRange range {};
            range.stageFlags = static_cast<VkShaderStageFlags>(item.stage_flags); // todo
            range.offset = item.offset;
            range.size = item.range;
            pushConstantRanges.push_back(range);
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
        pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

        if (VK_SUCCESS != vkCreatePipelineLayout(context->GetDevice(), &pipelineLayoutInfo, nullptr, &m_layout)) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        // TODO compute pipelines

        VkVertexInputBindingDescription vertexBindingDescription { 0, initializer.m_vertexLayout->GetStride(), VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX };

		std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions;
        auto& vertex_attribs = initializer.m_shader->VertexModule()->GetReflectionInfo()->VertexAttribs();
        for (auto& attrib : vertex_attribs) {
            VkVertexInputAttributeDescription desc;
            desc.location = attrib.location;
            desc.binding = 0;
            desc.format = static_cast<VkFormat>(initializer.m_vertexLayout->GetAttribFormat(attrib.vertex_attrib)); // todo
            vertexAttributeDescriptions.push_back(desc);
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = static_cast<VkPrimitiveTopology>(initializer.m_renderMode->GetTopology()); // todo
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = 0;
        viewport.height = 0;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor {};
        scissor.offset = { 0, 0 };
        scissor.extent = { 0, 0 };

        VkPipelineViewportStateCreateInfo viewportState {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = ToVulkan(initializer.m_renderMode->GetFillMode());
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = ToVulkan(initializer.m_renderMode->GetCullMode());
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisampling {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 0.0f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = initializer.m_renderMode->GetAlphaBlendEnabled();
        colorBlendAttachment.srcColorBlendFactor = ToVulkan(initializer.m_renderMode->GetSrcBlendFactor());
        colorBlendAttachment.dstColorBlendFactor = ToVulkan(initializer.m_renderMode->GetDstBlendFactor());
        colorBlendAttachment.colorBlendOp = ToVulkan(initializer.m_renderMode->GetBlendOp());
        colorBlendAttachment.srcAlphaBlendFactor = ToVulkan(initializer.m_renderMode->GetAlphaSrcBlendFactor());
        colorBlendAttachment.dstAlphaBlendFactor = ToVulkan(initializer.m_renderMode->GetAlphaDstBlendFactor());
        colorBlendAttachment.alphaBlendOp = ToVulkan(initializer.m_renderMode->GetAlphaBlendOp());

        VkPipelineColorBlendStateCreateInfo colorBlending {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineDepthStencilStateCreateInfo depthStencil {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = ToVulkan(initializer.m_renderMode->GetDepthTest());
        depthStencil.depthWriteEnable = ToVulkan(initializer.m_renderMode->GetDepthWrite());
        depthStencil.depthCompareOp = ToVulkan(initializer.m_renderMode->GetDepthFunc());
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};

		std::array<VkDynamicState, 2> dynamic_states = { VkDynamicState::VK_DYNAMIC_STATE_VIEWPORT, VkDynamicState::VK_DYNAMIC_STATE_SCISSOR };
        VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_info;
        pipeline_dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pipeline_dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
        pipeline_dynamic_state_info.pDynamicStates = dynamic_states.data();
        pipeline_dynamic_state_info.flags = 0;
        pipeline_dynamic_state_info.pNext = nullptr;

        VkGraphicsPipelineCreateInfo pipelineInfo {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = shaderStageCount;
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &pipeline_dynamic_state_info;
        pipelineInfo.layout = m_layout;
        pipelineInfo.renderPass = initializer.m_renderPass->GetRenderPass();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        if (VK_SUCCESS != vkCreateGraphicsPipelines(context->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline)) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
    }
}
