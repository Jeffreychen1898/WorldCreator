#include "geometryCache.h"

namespace GeometryCache
{
    /* geometry cache class */
    ShapesCache::ShapesCache()
    {
        m_shapes = new std::unordered_map<std::string, GeometryCache::Shapes*>();
    }

    bool ShapesCache::AppendShape(const char* _name, unsigned int _vertexSize, float* _vertices, unsigned int _indicesCount, unsigned int* _indices)
    {
        if(m_shapes->find(_name) != m_shapes->end())
            return false;
        
        GeometryCache::Shapes* new_shape = new GeometryCache::Shapes(_vertexSize, _indicesCount);
        new_shape->SetVertices(_vertices);
        new_shape->SetIndices(_indices);
        m_shapes->insert({ _name, new_shape });

        return true;
    }

    void ShapesCache::RemoveShape(const char* _name)
    {
        m_shapes->erase(_name);
    }

    bool ShapesCache::Rename(const char* _name, const char* _newname)
    {
        if(m_shapes->find(_name) == m_shapes->end())
            return true;
        
        if(m_shapes->find(_newname) != m_shapes->end())
            return false;

        GeometryCache::Shapes* get_shape = m_shapes->at(_name);
        m_shapes->erase(_name);
        m_shapes->insert({ _newname, get_shape });

        return true;
    }

    void ShapesCache::RenderShapes(Graphics::Renderer& _renderer)
    {
        for(const auto& each_shape : *m_shapes)
        {
            GeometryCache::Shapes* current_shape = each_shape.second;
            _renderer.DrawPolygons(current_shape->GetVerticesSize(), current_shape->GetVertices(), current_shape->GetIndicesCount(), current_shape->GetIndices());
        }
    }

    ShapesCache::~ShapesCache()
    {
        for(auto& each_shape : *m_shapes)
            delete each_shape.second;
        delete m_shapes;
    }

    /* shapes class */
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

    Shapes::~Shapes()
    {
        delete[] m_vertices;
        delete[] m_indices;
    }
}