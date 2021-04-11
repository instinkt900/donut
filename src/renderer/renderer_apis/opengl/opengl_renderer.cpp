#include "donut_pch.h"
#if defined(USE_OPENGL)
#include "opengl_renderer.h"

namespace donut::opengl {
    namespace {
        GLenum ToGL(PrimitiveType type) {
            switch (type) {
                case PrimitiveType::POINTS: return GL_POINTS;
                case PrimitiveType::LINES: return GL_LINES;
                case PrimitiveType::LINE_LOOP: return GL_LINE_LOOP;
                case PrimitiveType::LINE_STRIP: return GL_LINE_STRIP;
                case PrimitiveType::TRIANGLES: return GL_TRIANGLES;
                case PrimitiveType::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
                case PrimitiveType::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
                case PrimitiveType::QUADS: return GL_QUADS;
                default: assert(false); return GL_POINTS;
            }
        }

	void OpenGLMessageCallback(
            unsigned source,
            unsigned type,
            unsigned id,
            unsigned severity,
            int length,
            const char* message,
            const void* userParam) {
            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH: spdlog::error(message); return;
                case GL_DEBUG_SEVERITY_MEDIUM: spdlog::error(message); return;
                case GL_DEBUG_SEVERITY_LOW: spdlog::warn(message); return;
                case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::trace(message); return;
            }
        }
    }

    void Renderer::Init() {
#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer::Viewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void Renderer::Clear(glm::vec4 const& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::DrawPrimitives(PrimitiveType primitiveType, unsigned int start, unsigned int count) {
        glDrawArrays(ToGL(primitiveType), start, count);
    }

    void Renderer::DrawIndexedPrimitives(PrimitiveType primitiveType, unsigned int indexCount) {
        glDrawElements(ToGL(primitiveType), indexCount, GL_UNSIGNED_INT, 0);
    }
}

#endif
