#include "shape.h"

namespace GeometryCache
{
    /* shapes */
    Shapes::Shapes(unsigned int _verticesSize, unsigned int _indicesCount)
        : m_verticesSize(_verticesSize), m_indicesCount(_indicesCount)
    {
        m_vertices = new float[_verticesSize];
        m_indices = new unsigned int[_indicesCount];
    }

    void Shapes::SetVertices(float* _vertices)
    {
        memcpy(m_vertices, _vertices, sizeof(float) * m_verticesSize);
    }

    void Shapes::SetIndices(unsigned int* _indices)
    {
        memcpy(m_indices, _indices, sizeof(float) * m_indicesCount);
    }

    void Shapes::ReallocVerticesIndices()
    {
        delete[] m_vertices;
        delete[] m_indices;
        m_vertices = new float[m_verticesSize];
        m_indices = new unsigned int[m_indicesCount];
    }

    void Shapes::RotateZVertex(float _sx, float _sy, float _sz, float _position[], float _angle)
    {
        float vertex_x = _position[0];
        float vertex_y = _position[1];
        float vertex_z = _position[2];

        float cos_ang = std::cos(_angle);
        float sin_ang = std::sin(_angle);

        /* the following equations are from the rotation matrix with modification to spin around shape center instead of origin */
        _position[0] = cos_ang * (vertex_x - _sx) - sin_ang * (vertex_y - _sy) + _sx;
        _position[1] = sin_ang * (vertex_x - _sx) + cos_ang * (vertex_y - _sy) + _sy;
        _position[2] = vertex_z;
    }

    void Shapes::RotateYVertex(float _sx, float _sy, float _sz, float _position[], float _angle)
    {
        float vertex_x = _position[0];
        float vertex_y = _position[1];
        float vertex_z = _position[2];

        float cos_ang = std::cos(_angle);
        float sin_ang = std::sin(_angle);

        /* the following equations are from the rotation matrix with modification to spin around shape center instead of origin */
        _position[0] = cos_ang * (vertex_x - _sx) + sin_ang * (vertex_z - _sz) + _sx;
        _position[1] = vertex_y;
        _position[2] = -sin_ang * (vertex_x - _sx) + cos_ang * (vertex_z - _sz) + _sz;
    }

    void Shapes::RotateXVertex(float _sx, float _sy, float _sz, float _position[], float _angle)
    {
        float vertex_x = _position[0];
        float vertex_y = _position[1];
        float vertex_z = _position[2];

        float cos_ang = std::cos(_angle);
        float sin_ang = std::sin(_angle);

        /* the following equations are from the rotation matrix with modification to spin around shape center instead of origin */
        _position[0] = vertex_x;
        _position[1] = cos_ang * (vertex_y - _sy) - sin_ang * (vertex_z - _sz) + _sy;
        _position[2] = sin_ang * (vertex_y - _sy) + cos_ang * (vertex_z - _sz) + _sz;
    }

    Shapes::~Shapes()
    {
        delete[] m_vertices;
        delete[] m_indices;
    }

    /* planes */
    Planes::Planes()
        : Shapes(24, 6), m_x(0), m_y(0), m_z(0), m_length(100), m_width(100), m_rotateX(0), m_rotateY(0), m_rotateZ(0),
        m_horizontalSections(1), m_verticalSections(1)
    {
        Update();
        UpdateIndices();
    }

    void Planes::Update()
    {
        unsigned int horizontal_size = m_length / m_horizontalSections;
        unsigned int vertical_size = m_width / m_verticalSections;
        unsigned int vertex_count = (m_horizontalSections + 1) * (m_verticalSections + 1);

        glm::vec3 component_x = glm::vec3(1, 0, 0);
        glm::vec3 component_y = glm::vec3(0, 1, 0);
        glm::vec3 component_z = glm::vec3(0, 0, 1);

        // create the vertices
        for(unsigned int i=0;i<m_verticalSections + 1;++i)
        {
            for(unsigned int j=0;j<m_horizontalSections + 1;++j)
            {
                unsigned int vertices_tracker = i * (m_horizontalSections + 1) + j;
                Shapes::m_vertices[vertices_tracker * 6 + 0] = m_x + j * horizontal_size - m_length / 2.0;
                Shapes::m_vertices[vertices_tracker * 6 + 1] = m_y;
                Shapes::m_vertices[vertices_tracker * 6 + 2] = m_z + i * vertical_size - m_width / 2.0;
            }
        }
        
        // rotate the shape
        for(unsigned int i=0;i<vertex_count;++i)
        {
            Shapes::RotateXVertex(m_x, m_y, m_z, Shapes::m_vertices + (i * 6), m_rotateX);
            Shapes::RotateYVertex(m_x, m_y, m_z, Shapes::m_vertices + (i * 6), m_rotateY);
            Shapes::RotateZVertex(m_x, m_y, m_z, Shapes::m_vertices + (i * 6), m_rotateZ);
        }

        // calculate the normal
        for(unsigned int i=0;i<m_verticalSections + 1;++i)
        {
            for(unsigned int j=0;j<m_horizontalSections + 1;++j)
            {
                glm::vec3 up_direction = i > 0 ? glm::vec3(PositionAtIndex(i - 1, j) - PositionAtIndex(i, j)) : glm::vec3(0, 0, 0);
                glm::vec3 down_direction = i < m_verticalSections ? glm::vec3(PositionAtIndex(i + 1, j) - PositionAtIndex(i, j)) : glm::vec3(0, 0, 0);
                glm::vec3 left_direction = j > 0 ? glm::vec3(PositionAtIndex(i, j - 1) - PositionAtIndex(i, j)) : glm::vec3(0, 0, 0);
                glm::vec3 right_direction = j < m_horizontalSections ? glm::vec3(PositionAtIndex(i, j + 1) - PositionAtIndex(i, j)) : glm::vec3(0, 0, 0);

                glm::vec3 from_top = (i > 0 && j > 0) ? glm::normalize(glm::cross(up_direction, left_direction)) : glm::vec3(0, 0, 0);
                glm::vec3 from_left = (i < m_verticalSections && j > 0) ? glm::normalize(glm::cross(left_direction, down_direction)) : glm::vec3(0, 0, 0);
                glm::vec3 from_bottom = (i < m_verticalSections && j < m_horizontalSections) ? glm::normalize(glm::cross(down_direction, right_direction)) : glm::vec3(0, 0, 0);
                glm::vec3 from_right = (i > 0 && j < m_horizontalSections) ? glm::normalize(glm::cross(right_direction, up_direction)) : glm::vec3(0, 0, 0);

                glm::vec3 average_normal = glm::normalize(from_top + from_left + from_bottom + from_right);

                unsigned int index = (i * (m_horizontalSections + 1) + j) * 6;

                Shapes::m_vertices[index + 3] = glm::dot(average_normal, component_x);
                Shapes::m_vertices[index + 4] = glm::dot(average_normal, component_y);
                Shapes::m_vertices[index + 5] = glm::dot(average_normal, component_z);
            }
        }
    }

    void Planes::UpdateIndices()
    {
        for(unsigned int i=0;i<m_verticalSections;++i)
        {
            for(unsigned int j=0;j<m_horizontalSections;++j)
            {
                unsigned int counter = 6 * (i * m_horizontalSections + j);
                Shapes::m_indices[counter + 0] = (m_horizontalSections + 1) * i + j;
                Shapes::m_indices[counter + 1] = (m_horizontalSections + 1) * i + j + 1;
                Shapes::m_indices[counter + 2] = (m_horizontalSections + 1) * (i + 1) + j + 1;
                Shapes::m_indices[counter + 3] = (m_horizontalSections + 1) * i + j;
                Shapes::m_indices[counter + 4] = (m_horizontalSections + 1) * (i + 1) + j + 1;
                Shapes::m_indices[counter + 5] = (m_horizontalSections + 1) * (i + 1) + j;
            }
        }
    }

    void Planes::SetSections(unsigned int _horizontal, unsigned int _vertical)
    {
        m_horizontalSections = _horizontal;
        m_verticalSections = _vertical;

        unsigned int vertex_count = (m_horizontalSections + 1) * (m_verticalSections + 1);

        Shapes::m_verticesSize = 6 * vertex_count;
        Shapes::m_indicesCount = 3 * (vertex_count / 2.0);

        Shapes::ReallocVerticesIndices();

        Update();
        UpdateIndices();
    }

    glm::vec3 Planes::PositionAtIndex(unsigned int row, unsigned int col)
    {
        unsigned int horizontal_count = 1;
        unsigned int index = (row * (horizontal_count + 1) + col) * 6;

        glm::vec3 position = glm::vec3(
            Shapes::m_vertices[index + 0],
            Shapes::m_vertices[index + 1],
            Shapes::m_vertices[index + 2]
        );

        return position;
    }

    Planes::~Planes()
    {
        //
    }
}