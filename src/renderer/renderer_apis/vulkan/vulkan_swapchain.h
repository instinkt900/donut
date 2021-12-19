#pragma once

namespace donut::vulkan {
	class Context;

	class Swapchain {
	public:
		Swapchain(Context* context, uint32_t width, uint32_t height, Swapchain* oldSwapchain = nullptr);
		~Swapchain();

		VkSwapchainKHR GetSwapchain() const { return m_swapChain; }

	private:
		VkSwapchainKHR m_swapChain;
    	std::vector<VkImage> m_swapChainImages;
    	VkFormat m_swapChainImageFormat;
    	VkExtent2D m_swapChainExtent;
    	std::vector<VkImageView> m_swapChainImageViews;
	};
}
