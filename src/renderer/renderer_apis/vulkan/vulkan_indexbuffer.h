#pragma once
#if defined(USE_VULKAN)

namespace donut::vulkan {
    class IndexBuffer : public NonCopyable {
    public:
        static std::shared_ptr<IndexBuffer> Create(unsigned int* data, unsigned int count);
        ~IndexBuffer();
        unsigned int GetCount() const;
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_count = 0;

        IndexBuffer(unsigned int id, unsigned int count);
    };
}

#endif
