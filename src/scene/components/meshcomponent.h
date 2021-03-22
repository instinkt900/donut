#pragma once

#include "renderer/vertexbuffer.h"
#include "renderer/indexbuffer.h"

namespace donut {
    struct MeshComponent {
        PrimitiveType m_primitiveType;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<Texture2D> m_texture;
    };
}
