#pragma once

#include "renderer/vertexbuffer.h"
#include "renderer/indexbuffer.h"

namespace donut {
    namespace opengl {
        class Mesh {
        public:
            Mesh();
            Mesh(VertexBuffer vertexBuffer, IndexBuffer indexBuffer);
            ~Mesh() = default;

            void SetPrimitiveType(PrimitiveType type) { m_primitiveType = type; }
            PrimitiveType GetPrimitiveType() const { return m_primitiveType; }

            void Draw();

        private:
            VertexBuffer m_vertexBuffer;
            IndexBuffer m_indexBuffer;
            PrimitiveType m_primitiveType = PrimitiveType::TRIANGLES;

            Mesh(Mesh const&) = delete;
            Mesh& operator=(Mesh const&) = delete;
        };
    }
}
