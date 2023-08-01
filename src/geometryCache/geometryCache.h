#pragma once

#include <unordered_map>

#include "../graphics/render.h"

namespace GeometryCache
{
    class Shapes
    {
        private:
            float* m_vertices;
            unsigned int* m_indices;

            unsigned int m_verticesSize;
            unsigned int m_indicesCount;
        public:
            Shapes(unsigned int _verticesSize, unsigned int _indicesCount);
            ~Shapes();

            void SetVertices(float* _vertices);
            void SetIndices(unsigned int* _indices);

            unsigned int GetVerticesSize() const { return m_verticesSize; };
            unsigned int GetIndicesCount() const { return m_indicesCount; };

            float* GetVertices() { return m_vertices; };
            unsigned int* GetIndices() { return m_indices; };
    };

    class ShapesCache
    {
        private:
            std::unordered_map<std::string, GeometryCache::Shapes*>* m_shapes;

        public:
            ShapesCache();
            ~ShapesCache();

            bool AppendShape(const char* _name, unsigned int _vertexSize, float* _vertices, unsigned int _indicesCount, unsigned int* _indices);
            void RemoveShape(const char* _name);
            bool Rename(const char* _name, const char* _newname);

            void RenderShapes(Graphics::Renderer& _renderer);
    };
}