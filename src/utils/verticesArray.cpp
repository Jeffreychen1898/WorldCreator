#include "verticesArray.h"

namespace Utils
{
    VerticesArray::VerticesArray()
        : m_vertexCounter(0), m_indicesCounter(0), m_vertexSize(0)
    {
        m_indicesData = new unsigned int[MAX_INDICES];
        m_vertexData = new float[MAX_VERTEX_SIZE];
    }

    bool VerticesArray::AddShape(unsigned int _verticesSize, float* _vertices, unsigned int _indicesSize, unsigned int* _indices)
    {
        /* check that data is not overflowing */
        if(m_vertexCounter + _verticesSize >= MAX_VERTEX_SIZE)
            return false;
        if(m_indicesCounter + _indicesSize >= MAX_INDICES)
            return false;
        
        /* append the data to the array */
        unsigned int vertex_count = (unsigned int)(m_vertexCounter / m_vertexSize);

        memcpy(m_vertexData + m_vertexCounter, _vertices, sizeof(float) * _verticesSize);

        for(unsigned int i=0;i<_indicesSize;++i)
        {
            m_indicesData[i + m_indicesCounter] = _indices[i] + vertex_count;
        }

        m_vertexCounter += _verticesSize;
        m_indicesCounter += _indicesSize;

        return true;
    }

    void VerticesArray::Clear()
    {
        m_vertexCounter = 0;
        m_indicesCounter = 0;
    }

    VerticesArray::~VerticesArray()
    {
        delete[] m_indicesData;
        delete[] m_vertexData;
    }
}