#pragma once

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

#ifdef USE_OPENGL
#include "opengl/opengl_mesh.h"
namespace donut {
    using MeshImpl = opengl::Mesh;
}
#else
#error TODO
#endif
