#include "donut_pch.h"
#include "vulkan_renderpass.h"

#include "vulkan_utils.h"
#include "vulkan_context.h"

namespace donut::vulkan {
	uint32_t RenderPassInitializer::GetHash() const {
		std::array<uint32_t, 3> data;
		if (m_colorAttachments.size() > 0)
			data[0] = Hash(m_colorAttachments.data(), m_colorAttachments.size() * sizeof(VkAttachmentDescription));
		if (m_resolveAttachments.size() > 0)
			data[1] = Hash(m_resolveAttachments.data(), m_resolveAttachments.size() * sizeof(VkAttachmentDescription));
		if (m_hasDepth)
			data[2] = Hash(m_depthAttachment);
		return Hash(data);
	}

	RenderPass::RenderPass(Context* context, const RenderPassInitializer& initializer) {
		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkAttachmentReference> colorAttachmentReferences;
		std::vector<VkAttachmentReference> resolveAttachmentReferences;
		VkAttachmentReference depthAttachmentReference;

		for (auto&& attachment : initializer.m_colorAttachments) {
			attachments.push_back(attachment);
            VkAttachmentReference reference {};
            reference.attachment = static_cast<uint32_t>(attachments.size() - 1);
            reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachmentReferences.push_back(reference);
		}

		for (auto&& attachment : initializer.m_resolveAttachments) {
			attachments.push_back(attachment);
            VkAttachmentReference reference {};
            reference.attachment = static_cast<uint32_t>(attachments.size() - 1);
            reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			resolveAttachmentReferences.push_back(reference);
		}

		if (initializer.m_hasDepth) {
			attachments.push_back(initializer.m_depthAttachment);
			depthAttachmentReference = { static_cast<uint32_t>(attachments.size() - 1), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
		}

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = static_cast<uint32_t>(initializer.m_colorAttachments.size());
        subpass.pColorAttachments = colorAttachmentReferences.data();
        subpass.pDepthStencilAttachment = &depthAttachmentReference;
        subpass.pResolveAttachments = resolveAttachmentReferences.data();

        VkSubpassDependency dependency {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (VK_SUCCESS != vkCreateRenderPass(context->GetDevice(), &renderPassInfo, nullptr, &m_renderPass)) {
            throw std::runtime_error("failed to create render pass!");
        }

        m_hasDepth = initializer.m_hasDepth;
        m_hash = initializer.GetHash();
	}
}
