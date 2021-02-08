#pragma once

namespace donut {
    enum class PrimitiveType {
        UNKNOWN,
        POINTS,
        LINES,
        LINE_LOOP,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        QUADS
    };
}
