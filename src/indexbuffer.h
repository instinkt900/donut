#pragma once

namespace donut {
    class IndexBuffer {
    public:
        IndexBuffer() = default;

        IndexBuffer(unsigned int* data, unsigned int count) {
            unsigned int id = 0;
            glGenBuffers(1, &id);
            if (id != 0) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW);

                m_data = std::make_shared<Data>();
                m_data->m_id = id;
                m_data->m_count = count;
            }
        }

        ~IndexBuffer() = default;

        IndexBuffer(IndexBuffer const&) = default;
        IndexBuffer& operator=(IndexBuffer const&) = default;

        bool Valid() const { return m_data != nullptr; }
        operator bool() const { return Valid(); }

        unsigned int GetCount() const { 
            assert(Valid());
            return m_data->m_count;
        }

        void Bind() const {
            assert(Valid());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data->m_id);
        }

        void Unbind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

    private:
        struct Data {
            ~Data() { glDeleteBuffers(1, &m_id); }
            unsigned int m_id = 0;
            unsigned int m_count = 0;
        };

        std::shared_ptr<Data> m_data;
    };
}