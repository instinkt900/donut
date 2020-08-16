#pragma once

namespace donut {
    enum class VertexElementType {
        Float,
        Int,
        Uint,
        Short
    };

    struct VertexElement {
        VertexElementType m_type;
        int m_count;
        bool m_normalized;
    };

    inline size_t VertexElementTypeSize(VertexElementType const& type) {
        switch (type) {
            case VertexElementType::Float: return sizeof(float);
            case VertexElementType::Int: return sizeof(int);
            case VertexElementType::Uint: return sizeof(unsigned int);
            case VertexElementType::Short: return sizeof(short);
        }
        return 0;
    }
}
