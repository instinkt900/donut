#include "donut_pch.h"
#include "opengl_framebuffer.h"

namespace {
    bool IsDepthFormat(donut::FrameBufferTextureFormat format) {
        switch (format) {
            case donut::FrameBufferTextureFormat::Depth24Stencil8: return true;
        }
        return false;
    }
}

namespace donut::opengl {
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(FrameBufferDesc const& desc) {
        return std::make_shared<FrameBuffer>(desc);
    }

    FrameBuffer::FrameBuffer(FrameBufferDesc const& desc) {
        glCreateFramebuffers(1, &m_id);
        if (m_id != 0) {
            glBindFramebuffer(GL_FRAMEBUFFER, m_id);

            m_width = desc.m_width;
            m_height = desc.m_height;

            int colorBufferCount = 0;
            int depthBufferCount = 0;
            for (auto&& texDesc : desc.m_attachments) {
                if (IsDepthFormat(texDesc.m_format))
                    depthBufferCount++;
                else
                    colorBufferCount++;
            }

            m_colorTextures = Texture2D::Create(colorBufferCount, m_width, m_height, TextureFormat::RGBA);
            auto attachmentId = GL_COLOR_ATTACHMENT0;
            for (auto&& texture : m_colorTextures) {
                glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentId, GL_TEXTURE_2D, texture->GetId(), 0);
                ++attachmentId;
            }

            if (depthBufferCount > 0) {
                m_depthTexture = Texture2D::Create(m_width, m_height, TextureFormat::Depth);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->GetId(), 0);
                //glCreateTextures(GL_TEXTURE_2D, 1, &m_depthBufferId);
                //glBindTexture(GL_TEXTURE_2D, m_depthBufferId);
                //glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_width, m_height);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthBufferId, 0);
                //m_depthTexture = Texture2D::Create(m_depthBufferId);
            }

            if (m_colorTextures.size() > 1) {
                GLenum const buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
                glDrawBuffers(static_cast<int>(m_colorTextures.size()), buffers);
            } else if (m_colorTextures.empty()) {
                glDrawBuffer(GL_NONE);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &m_id);
        //glDeleteTextures(static_cast<int>(m_colorBufferIds.size()), m_colorBufferIds.data());
        //glDeleteTextures(1, &m_depthBufferId);
    }

    void FrameBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glViewport(0, 0, m_width, m_height);
    }

    void FrameBuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_width, m_height);
    }

    std::shared_ptr<Texture2D> FrameBuffer::GetColorTexture(unsigned int index) const {
        return m_colorTextures[index];
    }
}