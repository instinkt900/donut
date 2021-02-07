#include "donut_pch.h"
#include "mesh.h"

namespace donut {
    Mesh::Mesh() {
        m_impl = std::make_shared<MeshImpl>();
    }

    Mesh::Mesh(VertexBuffer vertexBuffer, IndexBuffer indexBuffer) {
        m_impl = std::make_shared<MeshImpl>(vertexBuffer, indexBuffer);
    }

    void Mesh::SetPrimitiveType(PrimitiveType type) {
        if (Valid())
            m_impl->SetPrimitiveType(type);
    }

    PrimitiveType Mesh::GetPrimitiveType() const {
        if (Valid())
            return m_impl->GetPrimitiveType();
        return PrimitiveType::UNKNOWN;
    }

    void Mesh::Draw() {
        if (Valid())
            m_impl->Draw();
    }
}