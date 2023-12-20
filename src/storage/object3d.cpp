#include "object3d.hpp"

namespace Storage
{
    Object3D::Object3D()
        : m_vertices(nullptr), m_shiftVectors(nullptr), m_indices(nullptr), m_normals(nullptr),
        m_verticesCount(0), m_indicesCount(0)
    {
    }

    void Object3D::SetObject3D(float* _vertices, float* _shiftVectors, unsigned int* _indices,
    unsigned int _verticesCount, unsigned int _indicesCount)
    {
        if(m_vertices != nullptr) delete[] m_vertices;
        if(m_shiftVectors != nullptr) delete[] m_shiftVectors;
        if(m_indices != nullptr) delete[] m_indices;
        if(m_normals != nullptr) delete[] m_normals;

        m_verticesCount = _verticesCount;
        m_indicesCount = _indicesCount;

        m_vertices = new float[_verticesCount];
        m_shiftVectors = new float[_verticesCount];
        m_indices = new unsigned int[_indicesCount];
        m_normals = new float[_verticesCount];

        memcpy(m_vertices, _vertices, sizeof(float) * _verticesCount);
        memcpy(m_shiftVectors, _shiftVectors, sizeof(float) * _verticesCount);
        memcpy(m_indices, _indices, sizeof(unsigned int) * _indicesCount);

        std::cout << "vertices\n";
        for(int i=0;i<_verticesCount;++i)
            std::cout << m_vertices[i] << "\n";
        std::cout << "indices\n";
        for(int i=0;i<_indicesCount;++i)
            std::cout << m_indices[i] << "\n";

        CalculateNormals();
    }

    void Object3D::CalculateNormals()
    {
        unsigned int* vertex_normal_count = new unsigned int[m_verticesCount];
        for(unsigned int i=0;i<m_verticesCount;++i)
            vertex_normal_count[i] = 0;

        for(unsigned int i=0;i<m_indicesCount / 3;++i)
        {
            unsigned int vertex_one_idx = m_indices[i * 3 + 0];
            unsigned int vertex_two_idx = m_indices[i * 3 + 1];
            unsigned int vertex_three_idx = m_indices[i * 3 + 2];

            glm::vec3 vertex_one_position = glm::vec3(
                m_vertices[3 * vertex_one_idx + 0],
                m_vertices[3 * vertex_one_idx + 1],
                m_vertices[3 * vertex_one_idx + 2]
            );
            glm::vec3 vertex_two_position = glm::vec3(
                m_vertices[3 * vertex_two_idx + 0],
                m_vertices[3 * vertex_two_idx + 1],
                m_vertices[3 * vertex_two_idx + 2]
            );
            glm::vec3 vertex_three_position = glm::vec3(
                m_vertices[3 * vertex_three_idx + 0],
                m_vertices[3 * vertex_three_idx + 1],
                m_vertices[3 * vertex_three_idx + 2]
            );

            glm::vec3 to_vertex_two = vertex_two_position - vertex_one_position;
            glm::vec3 to_vertex_three = vertex_three_position - vertex_one_position;
            glm::vec3 normal_vector = glm::normalize(glm::cross(to_vertex_three, to_vertex_two));

            SetNormalVector(m_vertices + 3 * vertex_one_idx, normal_vector, vertex_normal_count[vertex_one_idx]);
            SetNormalVector(m_vertices + 3 * vertex_two_idx, normal_vector, vertex_normal_count[vertex_two_idx]);
            SetNormalVector(m_vertices + 3 * vertex_three_idx, normal_vector, vertex_normal_count[vertex_three_idx]);

            ++ vertex_normal_count[vertex_one_idx];
            ++ vertex_normal_count[vertex_two_idx];
            ++ vertex_normal_count[vertex_three_idx];
        }

        delete vertex_normal_count;
    }

    void Object3D::SetNormalVector(float* _xComponent, glm::vec3& _normal,
    unsigned int _count)
    {
        float* x_component = _xComponent;
        float* y_component = _xComponent + 1;
        float* z_component = _xComponent + 2;

        _normal /= (_count + 1.f);
        glm::vec3 x_axis(1, 0, 0);
        glm::vec3 y_axis(0, 1, 0);
        glm::vec3 z_axis(0, 0, 1);

        float multiplier = static_cast<float>(_count) / (_count + 1.f);

        *x_component = (*x_component) * multiplier + dot(_normal, x_axis);
        *y_component = (*y_component) * multiplier + dot(_normal, y_axis);
        *z_component = (*z_component) * multiplier + dot(_normal, z_axis);
    }

    Object3D::~Object3D()
    {
        if(m_vertices != nullptr)
            delete[] m_vertices;
        if(m_shiftVectors != nullptr)
            delete[] m_shiftVectors;
        if(m_indices != nullptr)
            delete[] m_indices;
        if(m_normals != nullptr)
            delete[] m_normals;
    }
}