#include "donut_pch.h"
#include "opengl_mesh.h"

namespace donut {
    namespace opengl {
        namespace {
            GLenum ToGL(PrimitiveType type) {
                switch (type) {
                    case PrimitiveType::POINTS: return GL_POINTS;
                    case PrimitiveType::LINES: return GL_LINES;
                    case PrimitiveType::LINE_LOOP: return GL_LINE_LOOP;
                    case PrimitiveType::LINE_STRIP: return GL_LINE_STRIP;
                    case PrimitiveType::TRIANGLES: return GL_TRIANGLES;
                    case PrimitiveType::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
                    case PrimitiveType::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
                    case PrimitiveType::QUADS: return GL_QUADS;
                    default: assert(false); return GL_POINTS;
                }
            }
        }

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
                glDrawElements(ToGL(m_primitiveType), m_indexBuffer.GetCount(), GL_UNSIGNED_INT, 0);
                m_indexBuffer.Unbind();
                m_vertexBuffer.Unbind();
            }
        }
    }
}
