#pragma once

#include <iostream>
#include <unordered_map>
#include <cstring>
#include <cmath>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/io.hpp>

namespace GeometryCache
{
    class Shapes
    {
        protected:
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

            virtual void SetPosition(float _x, float _y, float _z) {};
            virtual void SetRotation(float _x, float _y, float _z) {};
            virtual void SetDimensions(float _length, float _width) {};

            virtual void Update() {};

        protected:
            static void RotateZVertex(float _sx, float _sy, float _sz, float _position[], float _angle);
            static void RotateYVertex(float _sx, float _sy, float _sz, float _position[], float _angle);
            static void RotateXVertex(float _sx, float _sy, float _sz, float _position[], float _angle);

            void ReallocVerticesIndices();
    };

    class Planes : public Shapes
    {
        private:
            float m_x;
            float m_y;
            float m_z;

            float m_length;
            float m_width;

            float m_rotateX;
            float m_rotateY;
            float m_rotateZ;

            unsigned int m_horizontalSections;
            unsigned int m_verticalSections;
        public:
            Planes();
            ~Planes();

            void SetPosition(float _x, float _y, float _z) { m_x = _x; m_y = _y; m_z = _z; };
            void SetRotation(float _x, float _y, float _z) { m_rotateX = _x; m_rotateY = _y; m_rotateZ = _z; };
            void SetDimensions(float _length, float _width) { m_width = _width; m_length = _length; };
            void SetSections(unsigned int _horizontal, unsigned int _vertical);

            void Update();
            void UpdateIndices();
        
        private:
            glm::vec3 PositionAtIndex(unsigned int row, unsigned int col);
    };
}