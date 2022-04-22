#pragma once

namespace donut::vulkan {
    enum class FillMode {
        Fill,
        Line,
        Point
    };

    enum class CullMode {
        Front,
        Back
    };

    enum class Topology {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        TriabgleFan,
    };

    enum class CompareOp {
        Never,
        Less,
        Equal,
        LessOrEqual,
        Greater,
        NotEqual,
        GreaterOrEqual,
        Always,
    };

    enum class BlendFactor {
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha,
        SrcAlphaSaturate,
        Src1Color,
        OneMinusSrc1Color,
        Src1Alpha,
        OneMinusSrc1Alpha,
    };

    enum class BlendOp {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    class RenderMode {
    public:
        uint32_t GetHash() const {
            if (m_hashDirty) {
                GenerateHash();
            }
            return m_hash;
        }

        void SetFillMode(FillMode mode) {
            if (m_fillMode != mode) {
                m_fillMode = mode;
                m_hashDirty = true;
            }
        }
        FillMode GetFillMode() const { return m_fillMode; }

        void SetCullMode(CullMode mode) {
            if (m_cullMode != mode) {
                m_cullMode = mode;
                m_hashDirty = true;
            }
        }
        CullMode GetCullMode() const { return m_cullMode; }

        void SetTopology(Topology topology) {
            if (m_topology != topology) {
                m_topology = topology;
                m_hashDirty = true;
            }
        }
        Topology GetTopology() const { return m_topology; }

        void SetDepthTestEnabled(bool enabled) {
            if (m_depthTest != enabled) {
                m_depthTest = enabled;
                m_hashDirty = true;
            }
        }
        bool GetDepthTest() const { return m_depthTest; }

        void SetDepthWriteEnabled(bool enabled) {
            if (m_depthWrite != enabled) {
                m_depthWrite = enabled;
                m_hashDirty = true;
            }
        }
        bool GetDepthWrite() const { return m_depthWrite; }

        void SetAlphaBlendEnabled(bool enabled) {
            if (m_alphaBlend != enabled) {
                m_alphaBlend = enabled;
                m_hashDirty = true;
            }
        }
        bool GetAlphaBlendEnabled() const { return m_alphaBlend; }

        void SetDepthFunc(CompareOp op) {
            if (m_depthFunc != op) {
                m_depthFunc = op;
                m_hashDirty = true;
            }
        }
        CompareOp GetDepthFunc() const { return m_depthFunc; }

        void SetSrcBlendFactor(BlendFactor factor) {
            if (m_srcBlendFactor != factor) {
                m_srcBlendFactor = factor;
                m_hashDirty = true;
            }
        }
        BlendFactor GetSrcBlendFactor() const { return m_srcBlendFactor; }

        void SetDstBlendFactor(BlendFactor factor) {
            if (m_dstBlendFactor != factor) {
                m_dstBlendFactor = factor;
                m_hashDirty = true;
            }
        }
        BlendFactor GetDstBlendFactor() const { return m_dstBlendFactor; }

        void SetBlendOp(BlendOp op) {
            if (m_blendOp != op) {
                m_blendOp = op;
                m_hashDirty = true;
            }
        }
        BlendOp GetBlendOp() const { return m_blendOp; }

        void SetAlphaSrcBlendFactor(BlendFactor factor) {
            if (m_srcAlphaBlendFactor != factor) {
                m_srcAlphaBlendFactor = factor;
                m_hashDirty = true;
            }
        }
        BlendFactor GetAlphaSrcBlendFactor() const { return m_srcAlphaBlendFactor; }

        void SetAlphaDstBlendFactor(BlendFactor factor) {
            if (m_dstAlphaBlendFactor != factor) {
                m_dstAlphaBlendFactor = factor;
                m_hashDirty = true;
            }
        }
        BlendFactor GetAlphaDstBlendFactor() const { return m_dstAlphaBlendFactor; }

        void SetAlphaBlendOp(BlendOp op) {
            if (m_alphaBlendOp != op) {
                m_alphaBlendOp = op;
                m_hashDirty = true;
            }
        }
        BlendOp GetAlphaBlendOp() const { return m_alphaBlendOp; }

    private:
        FillMode m_fillMode;
        CullMode m_cullMode;
        Topology m_topology;
        bool m_depthTest;
        bool m_depthWrite;
        bool m_alphaBlend;
        CompareOp m_depthFunc;
        BlendFactor m_srcBlendFactor;
        BlendFactor m_dstBlendFactor;
        BlendOp m_blendOp;
        BlendFactor m_srcAlphaBlendFactor;
        BlendFactor m_dstAlphaBlendFactor;
        BlendOp m_alphaBlendOp;

        mutable bool m_hashDirty = true;
        mutable uint32_t m_hash = 0;

        void GenerateHash() const;
    };
}