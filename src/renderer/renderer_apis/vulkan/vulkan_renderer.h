#pragma once
#if defined(USE_VULKAN)

#include "renderer/primitivetype.h"

namespace donut::vulkan {
    class Renderer : public Abstract {
    public:
        static VkInstance GetInstance();
        static void Init();
        static void Shutdown();
        static void Viewport(int x, int y, int width, int height);
        static void Clear(glm::vec4 const& color);
        static void DrawPrimitives(PrimitiveType primitiveType, unsigned int start, unsigned int count);
        static void DrawIndexedPrimitives(PrimitiveType primitiveType, unsigned int indexCount);

        static void OnResize();
    };
}

#endif
