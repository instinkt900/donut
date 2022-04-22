#pragma once
#if defined(USE_VULKAN)

#include "renderer/vertexlayout.h"

namespace donut::vulkan {
    class VertexBuffer : public NonCopyable {
    public:
        static std::shared_ptr<VertexBuffer> Create(VertexLayout const& layout, void* data, unsigned int count);
        ~VertexBuffer();
        void Bind() const;
        void Unbind() const;

    private:
        VertexBuffer(unsigned int vertexBuffer, unsigned int vertexArray);
    };
}

#endif
