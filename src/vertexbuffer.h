#pragma once

#include "vertexlayout.h"

namespace donut {
    class VertexBuffer {
    public:
        VertexBuffer() = default;

        VertexBuffer(VertexLayout layout, void* data, unsigned int count) {
            unsigned int vertexArray = 0;
            glGenVertexArrays(1, &vertexArray);
            if (vertexArray != 0) {
                unsigned int vertexBuffer = 0;
                glBindVertexArray(vertexArray);
                glGenBuffers(1, &vertexBuffer);
                if (vertexBuffer == 0) {
                    glDeleteVertexArrays(1, &vertexArray);
                } else {
                    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                    glBufferData(GL_ARRAY_BUFFER, layout.GetStride() * count, data, GL_STATIC_DRAW);

                    layout.Apply();

                    m_data = std::make_shared<Data>();
                    m_data->m_vertexBuffer = vertexBuffer;
                    m_data->m_vertexArray = vertexArray;
                }
            }
        }

        ~VertexBuffer() = default;

        VertexBuffer(VertexBuffer const&) = default;
        VertexBuffer& operator=(VertexBuffer const&) = default;

        bool Valid() const { return nullptr != m_data; }
        operator bool() const { return Valid(); }

        void Bind() const {
            assert(Valid());
            glBindVertexArray(m_data->m_vertexArray);
        }

        void Unbind() const {
            glBindVertexArray(0);
        }

    private:
        struct Data {
            ~Data() {
                glDeleteVertexArrays(1, &m_vertexArray);
                glDeleteBuffers(1, &m_vertexBuffer);
            }
            unsigned int m_vertexBuffer = 0;
            unsigned int m_vertexArray = 0;
        };

        std::shared_ptr<Data> m_data;
    };
}