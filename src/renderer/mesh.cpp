#include "donut_pch.h"
#include "mesh.h"

namespace donut {
    Mesh::Mesh() {

    }

    Mesh::Mesh(VertexBuffer vertexBuffer, IndexBuffer indexBuffer)
        : m_vertexBuffer(vertexBuffer)
        , m_indexBuffer(indexBuffer) {

    }

    void Mesh::Draw() {
        if (m_vertexBuffer && m_indexBuffer) {
            m_vertexBuffer.Bind();
            m_indexBuffer.Bind();
            glDrawElements(GL_TRIANGLES, m_indexBuffer.GetCount(), GL_UNSIGNED_INT, 0);
            m_indexBuffer.Unbind();
            m_vertexBuffer.Unbind();
        }
    }
}