#include "donut_pch.h"
#include "vulkan_rendermode.h"

#include "vulkan_utils.h"

namespace donut::vulkan {
    void RenderMode::GenerateHash() const {
        std::array<uint32_t, 13> data;
        data[0] = static_cast<uint32_t>(m_fillMode);
        data[1] = static_cast<uint32_t>(m_cullMode);
        data[2] = static_cast<uint32_t>(m_topology);
        data[3] = static_cast<uint32_t>(m_depthTest);
        data[4] = static_cast<uint32_t>(m_depthWrite);
        data[5] = static_cast<uint32_t>(m_alphaBlend);
        data[6] = static_cast<uint32_t>(m_depthFunc);
        data[7] = static_cast<uint32_t>(m_srcBlendFactor);
        data[8] = static_cast<uint32_t>(m_dstBlendFactor);
        data[9] = static_cast<uint32_t>(m_blendOp);
        data[10] = static_cast<uint32_t>(m_srcAlphaBlendFactor);
        data[11] = static_cast<uint32_t>(m_dstAlphaBlendFactor);
        data[12] = static_cast<uint32_t>(m_alphaBlendOp);
        m_hash = Hash(data);
        m_hashDirty = false;
    }
}
