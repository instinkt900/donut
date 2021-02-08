#include "donut_pch.h"
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
    }

    void Renderer::Viewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void Renderer::Clear(glm::vec4 const& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::DrawPrimitives(PrimitiveType primitiveType, unsigned int start, unsigned int count) {
        glDrawArrays(ToGL(primitiveType), start, count);
    }

    void Renderer::DrawIndexedPrimitives(PrimitiveType primitiveType, unsigned int indexCount) {
        glDrawElements(ToGL(primitiveType), indexCount, GL_UNSIGNED_INT, 0);
    }
}