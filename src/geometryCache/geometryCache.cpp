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

    bool ShapesCache::AppendPlane(const char* _name, float _x, float _y, float _z)
    {
        if(m_shapes->find(_name) != m_shapes->end())
            return false;
        
        GeometryCache::Planes* new_plane = new GeometryCache::Planes();
        new_plane->SetPosition(_x, _y, _z);
        new_plane->SetRotation(0, 0, 0);
        new_plane->SetDimensions(200, 200);
        new_plane->Update();
        new_plane->UpdateIndices();
        m_shapes->insert({ _name, new_plane });
        return true;
    }

    void ShapesCache::SetShapeRotation(const char* _name, float _x, float _y, float _z)
    {
        if(m_shapes->find(_name) == m_shapes->end())
            return;
        
        m_shapes->at(_name)->SetRotation(_x, _y, _z);
        m_shapes->at(_name)->Update();
    }

    ShapesCache::~ShapesCache()
    {
        for(auto& each_shape : *m_shapes)
            delete each_shape.second;
        delete m_shapes;
    }
}