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
}