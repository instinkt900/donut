#pragma once

#ifdef USE_OPENGL
#include "opengl/opengl_indexbuffer.h"
namespace donut {
	using IndexBufferImpl = opengl::IndexBufferImpl;
}
#else
#error Todo
#endif
