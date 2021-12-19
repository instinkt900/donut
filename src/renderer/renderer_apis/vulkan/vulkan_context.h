#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace donut {
	class IWindow;
}

namespace donut::vulkan {
	class Context {
	public:
		Context(IWindow* window);
		~Context();

		VkSurfaceKHR GetSurface() const { return m_surface; }
		VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
		VkDevice GetDevice() const { return m_device; }
		
		VmaAllocator GetAllocator() { return m_allocator; }

	private:
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		VkDevice m_device;
	    VkQueue m_graphicsQueue;
    	VkQueue m_presentQueue;

		VmaAllocator m_allocator;

	    std::vector<VkSemaphore> m_imageAvailableSemaphores;
	    std::vector<VkSemaphore> m_renderFinishedSemaphores;
	    std::vector<VkFence> m_inFlightFences;

		VkDebugUtilsMessengerEXT m_debugMessenger;

		void CreateInstance();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSyncObjects();
		void SetupDebugMessenger();
	};
}
