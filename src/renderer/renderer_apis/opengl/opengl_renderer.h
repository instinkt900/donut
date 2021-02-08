#pragma once

#include "renderer/primitivetype.h"

namespace donut::opengl {
    class Renderer : public Abstract {
    public:
        static void Viewport(int x, int y, int width, int height);
        static void Clear(glm::vec4 const& color);
        static void DrawPrimitives(PrimitiveType primitiveType, unsigned int start, unsigned int count);
        static void DrawIndexedPrimitives(PrimitiveType primitiveType, unsigned int indexCount);
    };
}