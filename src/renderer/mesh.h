#pragma once

namespace donut {
	class Mesh {
    public:
        Mesh();
        Mesh(VertexBuffer vertexBuffer, IndexBuffer indexBuffer);

        Mesh(Mesh const&) = default;
        Mesh& operator=(Mesh const&) = default;

        ~Mesh() = default;

        void Draw();

    private:
        VertexBuffer m_vertexBuffer;
        IndexBuffer m_indexBuffer;
	};
}