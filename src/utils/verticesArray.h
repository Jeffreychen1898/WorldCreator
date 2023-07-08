#pragma once

#include <cstring>

#include "helperFunctions.h"

#include "../graphics/opengl/shader.h"

namespace Utils
{
    class VerticesArray
    {
        private:
            unsigned int* m_indicesData;
            float* m_vertexData;

            unsigned int m_vertexCounter;
            unsigned int m_indicesCounter;

            unsigned int m_vertexSize;

        public:
            VerticesArray();
            ~VerticesArray();
            
            void SetVertexSize(unsigned int _vertexSize) { m_vertexSize = _vertexSize; };
            bool AddShape(unsigned int _verticesSize, float* _vertices, unsigned int _indicesSize, unsigned int* _indices);
            void Clear();

            bool IsEmpty() const { return m_indicesCounter < 3; };
            unsigned int GetVertexCount() const { return m_vertexCounter; };
            unsigned int GetIndicesCount() const { return m_indicesCounter; };
            float* GetVerticesData() { return m_vertexData; };
            unsigned int* GetIndicesData() { return m_indicesData; };
            unsigned int GetVertexSize() const { return m_vertexSize; };
    };
}