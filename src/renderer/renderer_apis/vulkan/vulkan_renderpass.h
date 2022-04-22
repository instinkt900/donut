#pragma once

namespace donut::vulkan {
	class Context;

	class RenderPassInitializer {
	public:
		uint32_t GetHash() const;

		std::vector<VkAttachmentDescription> m_colorAttachments;
		std::vector<VkAttachmentDescription> m_resolveAttachments;
		VkAttachmentDescription m_depthAttachment;
		bool m_hasDepth;
	};

	class RenderPass {
	public:
		RenderPass(Context* context, const RenderPassInitializer& initializer);

		VkRenderPass GetRenderPass() const { return m_renderPass; }

		uint32_t GetHash() const { return m_hash; }

	private:
		uint32_t m_hash;
		bool m_hasDepth;
		VkRenderPass m_renderPass;
	};
}