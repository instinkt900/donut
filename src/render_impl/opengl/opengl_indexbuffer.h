#pragma once

namespace donut::opengl {
    class IndexBufferImpl : public NonCopyable {
    public:
        static std::shared_ptr<IndexBufferImpl> Create(unsigned int* data, unsigned int count);
        ~IndexBufferImpl();
        unsigned int GetCount() const;
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_id = 0;
        unsigned int m_count = 0;

        IndexBufferImpl(unsigned int id, unsigned int count);
    };
}