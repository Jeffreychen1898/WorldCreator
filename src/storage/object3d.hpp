#pragma once

#include <string>
#include <cstring>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace Storage
{
    class Object3D
    {
        private:
            float* m_vertices;
            float* m_shiftVectors;
            unsigned int* m_indices;
            float* m_normals;

            unsigned int m_verticesCount;
            unsigned int m_indicesCount;

            /*float m_rotationX;
            float m_rotationY;
            float m_rotationZ;
            // mat4 m_modelMatrix*/
        public:
            Object3D();
            ~Object3D();

            void SetObject3D(float* _vertices, float* _shiftVectors, unsigned int* _indices,
            unsigned int _verticesCount, unsigned int _indicesCount);
        
        private:
            void CalculateNormals();
            void SetNormalVector(float* _xComponent, glm::vec3& _normal, unsigned int _count);
    };
}