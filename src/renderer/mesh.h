#pragma once

#include "render_impl/mesh_impl.h"

namespace donut {
	class Mesh {
    public:
        Mesh();
        Mesh(VertexBuffer vertexBuffer, IndexBuffer indexBuffer);
        Mesh(Mesh const&) = default;
        Mesh& operator=(Mesh const&) = default;
        ~Mesh() = default;

        bool Valid() const { return m_impl != nullptr; }
        operator bool() const { return Valid(); }

        void SetPrimitiveType(PrimitiveType type);
        PrimitiveType GetPrimitiveType() const;

        void Draw();

    private:
        std::shared_ptr<MeshImpl> m_impl;
	};
}