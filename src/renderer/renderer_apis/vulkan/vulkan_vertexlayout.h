#pragma once
#if defined(USE_VULKAN)

#include "renderer/vertexelement.h"

namespace donut::vulkan {
    class VertexLayout : public NonCopyable {
    public:
        VertexLayout(std::initializer_list<VertexElement> elements);
        ~VertexLayout() = default;

        size_t GetStride() const { return 0; }

        void Apply() const;

    private:
    };
}

#endif
