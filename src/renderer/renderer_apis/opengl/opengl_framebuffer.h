#pragma once

namespace donut::opengl {
    class FrameBuffer : public NonCopyable {
    public:
        static std::shared_ptr<FrameBuffer> Create(FrameBufferDesc const& desc);
        FrameBuffer(FrameBufferDesc const& desc);
        ~FrameBuffer();

        void Bind();
        void Unbind();

        std::shared_ptr<Texture2D> GetColorTexture(unsigned int index) const;

    private:
        unsigned int m_id = 0;
        unsigned int m_width = 0;
        unsigned int m_height = 0;

        //std::vector<unsigned int> m_colorBufferIds;
        //unsigned int m_depthBufferId = 0;

        std::vector<std::shared_ptr<Texture2D>> m_colorTextures;
        std::shared_ptr<Texture2D> m_depthTexture;
    };
}