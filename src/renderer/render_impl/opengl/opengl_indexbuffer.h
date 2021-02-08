#pragma once

namespace donut::opengl {
    class IndexBuffer : public NonCopyable {
    public:
        static std::shared_ptr<IndexBuffer> Create(unsigned int* data, unsigned int count);
        ~IndexBuffer();
        unsigned int GetCount() const;
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_id = 0;
        unsigned int m_count = 0;

        IndexBuffer(unsigned int id, unsigned int count);
    };
}