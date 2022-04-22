#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include <iostream>

VkAllocationCallbacks* g_vkAllocator = nullptr;
VkInstance g_vkInstance = VK_NULL_HANDLE;
VkPhysicalDevice g_vkPhysicalDevice = VK_NULL_HANDLE;
uint32_t g_vkQueueFamily = static_cast<uint32_t>(-1);
VkDevice g_vkDevice = VK_NULL_HANDLE;
VkQueue g_vkQueue = VK_NULL_HANDLE;
VkDescriptorPool g_vkDescriptorPool = VK_NULL_HANDLE;
ImGui_ImplVulkanH_Window g_imWindowData;
int g_imMinImageCount = 2;
VkPipelineCache g_vkPipelineCache = VK_NULL_HANDLE;
bool g_vkSwapChainrebuild = false;

#define CHECK_VK_RESULT(expr)                                                                                \
    {                                                                                                        \
        VkResult result_ = expr;                                                                             \
        if (result_ != VK_SUCCESS) {                                                                         \
            std::cerr << "File: " << __FILE__ << " Line: " << __LINE__ << #expr << " = " << result_ << "\n"; \
            abort();                                                                                         \
        }                                                                                                    \
    }

#define ARRAYSIZE(arr) ((int)(sizeof(arr) / sizeof(*(arr))))

void checkVkResult(VkResult err) {
    CHECK_VK_RESULT(err);
}

void initVulkan() {
    {
        uint32_t extensionsCount = 0;
        const char** extensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
        VkInstanceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.enabledExtensionCount = extensionsCount;
        createInfo.ppEnabledExtensionNames = extensions;
        CHECK_VK_RESULT(vkCreateInstance(&createInfo, g_vkAllocator, &g_vkInstance));
    }

    {
        uint32_t gpuCount;
        CHECK_VK_RESULT(vkEnumeratePhysicalDevices(g_vkInstance, &gpuCount, nullptr));
        std::vector<VkPhysicalDevice> gpus(gpuCount);
        CHECK_VK_RESULT(vkEnumeratePhysicalDevices(g_vkInstance, &gpuCount, gpus.data()));

        uint32_t selectedGpu = 0;
        for (uint32_t i = 0; i < gpuCount; ++i) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(gpus[i], &properties);
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                selectedGpu = i;
                break;
            }
        }

        g_vkPhysicalDevice = gpus[selectedGpu];
    }

    {
        uint32_t queueCount;
        vkGetPhysicalDeviceQueueFamilyProperties(g_vkPhysicalDevice, &queueCount, nullptr);
        std::vector<VkQueueFamilyProperties> queues(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(g_vkPhysicalDevice, &queueCount, queues.data());

        for (uint32_t i = 0; i < queueCount; ++i) {
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                g_vkQueueFamily = i;
                break;
            }
        }
    }

    {
        int deviceExtensionCount = 1;
        char const* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        float const queuePriority[] = { 1.0f };
        VkDeviceQueueCreateInfo queueInfo[1] = {};
        queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo[0].queueFamilyIndex = g_vkQueueFamily;
        queueInfo[0].queueCount = 1;
        queueInfo[0].pQueuePriorities = queuePriority;
        VkDeviceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = ARRAYSIZE(queueInfo);
        createInfo.pQueueCreateInfos = queueInfo;
        createInfo.enabledExtensionCount = deviceExtensionCount;
        createInfo.ppEnabledExtensionNames = deviceExtensions;
        CHECK_VK_RESULT(vkCreateDevice(g_vkPhysicalDevice, &createInfo, g_vkAllocator, &g_vkDevice));
        vkGetDeviceQueue(g_vkDevice, g_vkQueueFamily, 0, &g_vkQueue);
    }

    {
        VkDescriptorPoolSize poolSizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 },
        };
        VkDescriptorPoolCreateInfo poolInfo {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * ARRAYSIZE(poolSizes);
        poolInfo.poolSizeCount = static_cast<uint32_t>(ARRAYSIZE(poolSizes));
        poolInfo.pPoolSizes = poolSizes;
        CHECK_VK_RESULT(vkCreateDescriptorPool(g_vkDevice, &poolInfo, g_vkAllocator, &g_vkDescriptorPool));
    }
}

void initVulkanWindow(VkSurfaceKHR vkSurface, int width, int height) {
    g_imWindowData.Surface = vkSurface;

    VkBool32 result;
    vkGetPhysicalDeviceSurfaceSupportKHR(g_vkPhysicalDevice, g_vkQueueFamily, g_imWindowData.Surface, &result);
    if (result != VK_TRUE) {
        std::cerr << "No WSI support\n";
        exit(-1);
    }

    VkFormat const requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_R8G8B8_UNORM, VK_FORMAT_B8G8R8_UNORM };
    VkColorSpaceKHR const requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    g_imWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_vkPhysicalDevice, g_imWindowData.Surface, requestSurfaceImageFormat, static_cast<size_t>(ARRAYSIZE(requestSurfaceImageFormat)), requestSurfaceColorSpace);

    VkPresentModeKHR presentModes[] = { VK_PRESENT_MODE_FIFO_KHR };
    g_imWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_vkPhysicalDevice, g_imWindowData.Surface, presentModes, ARRAYSIZE(presentModes));

    ImGui_ImplVulkanH_CreateOrResizeWindow(g_vkInstance, g_vkPhysicalDevice, g_vkDevice, &g_imWindowData, g_vkQueueFamily, g_vkAllocator, width, height, g_imMinImageCount);
}

void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* drawData) {
    VkResult result;

    VkSemaphore imageAcquiredSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore renderCompleteSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    result = vkAcquireNextImageKHR(g_vkDevice, wd->Swapchain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &wd->FrameIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        g_vkSwapChainrebuild = true;
        return;
    }
    CHECK_VK_RESULT(result);

    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
    {
        CHECK_VK_RESULT(vkWaitForFences(g_vkDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX));
        CHECK_VK_RESULT(vkResetFences(g_vkDevice, 1, &fd->Fence));
    }

    {
        CHECK_VK_RESULT(vkResetCommandPool(g_vkDevice, fd->CommandPool, 0));
        VkCommandBufferBeginInfo info {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        CHECK_VK_RESULT(vkBeginCommandBuffer(fd->CommandBuffer, &info));
    }

    {
        VkRenderPassBeginInfo info {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO;
        info.renderPass = wd->RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = wd->Width;
        info.renderArea.extent.height = wd->Height;
        info.clearValueCount = 1;
        info.pClearValues = &wd->ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    ImGui_ImplVulkan_RenderDrawData(drawData, fd->CommandBuffer);

    vkCmdEndRenderPass(fd->CommandBuffer);

    {
        VkPipelineStageFlags waitStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &imageAcquiredSemaphore;
        info.pWaitDstStageMask = &waitStageFlags;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &renderCompleteSemaphore;
        CHECK_VK_RESULT(vkEndCommandBuffer(fd->CommandBuffer));
        CHECK_VK_RESULT(vkQueueSubmit(g_vkQueue, 1, &info, fd->Fence));
    }
}

void FramePresent(ImGui_ImplVulkanH_Window* wd) {
    if (g_vkSwapChainrebuild) {
        return;
    }

    VkSemaphore renderCompleteSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &renderCompleteSemaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &wd->Swapchain;
    info.pImageIndices = &wd->FrameIndex;
    VkResult result = vkQueuePresentKHR(g_vkQueue, &info);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        g_vkSwapChainrebuild = true;
        return;
    }
    CHECK_VK_RESULT(result);
    wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount;
}

int main(int argc, char* argv[]) {
    if (!glfwInit()) {
        return -1;
    }

    if (!glfwVulkanSupported()) {
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto window = glfwCreateWindow(640, 480, "Testing", nullptr, nullptr);

    initVulkan();

    VkSurfaceKHR vkSurface;
    CHECK_VK_RESULT(glfwCreateWindowSurface(g_vkInstance, window, g_vkAllocator, &vkSurface));

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    initVulkanWindow(vkSurface, width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo initInfo {};
    initInfo.Instance = g_vkInstance;
    initInfo.PhysicalDevice = g_vkPhysicalDevice;
    initInfo.Device = g_vkDevice;
    initInfo.QueueFamily = g_vkQueueFamily;
    initInfo.Queue = g_vkQueue;
    initInfo.PipelineCache = g_vkPipelineCache;
    initInfo.DescriptorPool = g_vkDescriptorPool;
    initInfo.Subpass = 0;
    initInfo.MinImageCount = g_imMinImageCount;
    initInfo.ImageCount = g_imWindowData.ImageCount;
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    initInfo.Allocator = g_vkAllocator;
    initInfo.CheckVkResultFn = checkVkResult;
    ImGui_ImplVulkan_Init(&initInfo, g_imWindowData.RenderPass);

    {
        VkCommandPool commandPool = g_imWindowData.Frames[g_imWindowData.FrameIndex].CommandPool;
        VkCommandBuffer commandBuffer = g_imWindowData.Frames[g_imWindowData.FrameIndex].CommandBuffer;
        CHECK_VK_RESULT(vkResetCommandPool(g_vkDevice, commandPool, 0));
        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        CHECK_VK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        VkSubmitInfo endInfo {};
        endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        endInfo.commandBufferCount = 1;
        endInfo.pCommandBuffers = &commandBuffer;
        CHECK_VK_RESULT(vkEndCommandBuffer(commandBuffer));
        CHECK_VK_RESULT(vkQueueSubmit(g_vkQueue, 1, &endInfo, VK_NULL_HANDLE));
        CHECK_VK_RESULT(vkDeviceWaitIdle(g_vkDevice));

        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (g_vkSwapChainrebuild) {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            if (width > 0 && height > 0) {
                ImGui_ImplVulkan_SetMinImageCount(g_imMinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(g_vkInstance, g_vkPhysicalDevice, g_vkDevice, &g_imWindowData, g_vkQueueFamily, g_vkAllocator, width, height, g_imMinImageCount);
                g_imWindowData.FrameIndex = 0;
                g_vkSwapChainrebuild = false;
            }
        }

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
                windowFlags |= ImGuiWindowFlags_NoBackground;
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Dockspace Demo", nullptr, windowFlags);
            ImGui::PopStyleVar();

            ImGui::PopStyleVar(2);

            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspaceId = ImGui::GetID("VulkanAppDockspace");
                ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
            }

            ImGui::ShowDemoWindow();

            ImGui::End();
        }

        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        FrameRender(&g_imWindowData, drawData);

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        FramePresent(&g_imWindowData);
    }
}
