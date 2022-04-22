#pragma once
#if defined(USE_VULKAN)

namespace donut::vulkan {
    class FrameBuffer : public NonCopyable {
    public:
        static std::shared_ptr<FrameBuffer> Create(FrameBufferDesc const& desc);
        FrameBuffer(FrameBufferDesc const& desc);
        ~FrameBuffer();

        void Bind();
        void Unbind();

        std::shared_ptr<Texture2D> GetColorTexture(unsigned int index) const;

    private:
    };
}

#endif
