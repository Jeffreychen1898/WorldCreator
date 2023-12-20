#include "storage.hpp"

namespace Storage
{
    static std::unordered_map<std::string, Graphics::Texture*>* m_textures;
    static std::unordered_map<std::string, Object3D*>* m_objects;

    Object3D* GetObject(std::string& _name)
    {
        return nullptr;
    }

    Graphics::Texture* GetTexture(std::string& _name)
    {
        auto find_texture = m_textures->find(_name);
        if(find_texture == m_textures->end())
            return nullptr;

        return find_texture->second;
    }

    void GetObjectsList(std::list<Object3D*>& _list)
    {
        for(auto it = m_objects->begin(); it != m_objects->end(); ++it)
            _list.push_back(it->second);
    }

    void CreatePlane(std::string& _name, float _width, float _height,
    unsigned int _sectionX, unsigned int _sectionY)
    {
        auto find_object = m_objects->find(_name);
        if(find_object != m_objects->end())
            return;
        
        // construct the object
        float section_width = _width / _sectionX;
        float section_height = _height / _sectionY;

        int vertex_x_count = _sectionX + 1;
        int vertex_y_count = _sectionY + 1;

        float* object_vertices = new float[vertex_x_count * vertex_y_count * 3];
        float* shift_vectors = new float[vertex_x_count * vertex_y_count * 3];
        unsigned int* object_indices = new unsigned int[2 * _sectionX * _sectionY * 3];

        // generate the vertices and the shift vectors
        for(int i=0;i<vertex_y_count;++i)
        {
            for(int j=0;j<vertex_x_count;++j)
            {
                int get_index = i * vertex_x_count + j;
                object_vertices[get_index * 3 + 0] = section_width * j;
                object_vertices[get_index * 3 + 1] = 0;
                object_vertices[get_index * 3 + 2] = section_height * i;

                shift_vectors[get_index * 3 + 0] = 0;
                shift_vectors[get_index * 3 + 1] = 1;
                shift_vectors[get_index * 3 + 2] = 0;
            }
        }

        // generate the indices
        for(int i=0;i<_sectionY;++i)
        {
            for(int j=0;j<_sectionX;++j)
            {
                unsigned int get_index = i * _sectionX + j;
                object_indices[get_index * 6 + 0] = _sectionX * i + j;
                object_indices[get_index * 6 + 1] = _sectionX * i + j + 1;
                object_indices[get_index * 6 + 2] = _sectionX * (i + 1) + j + 1;
                object_indices[get_index * 6 + 3] = _sectionX * i + j;
                object_indices[get_index * 6 + 4] = _sectionX * (i + 1) + j + 1;
                object_indices[get_index * 6 + 5] = _sectionX * (i + 1) + j;
            }
        }

        Object3D* new_object = new Object3D();
        new_object->SetObject3D(object_vertices, shift_vectors, object_indices,
        vertex_x_count * vertex_y_count * 3, 2 * _sectionX * _sectionY * 3);
        m_objects->insert({ _name, new_object });

        delete[] object_vertices;
        delete[] shift_vectors;
        delete[] object_indices;
    }

    void CreateTexture(std::string& _name, unsigned int _channels, unsigned int _width,
        unsigned int _height, unsigned char* _data, unsigned char _settings)
    {
        auto find_texture = m_textures->find(_name);
        if(find_texture != m_textures->end())
            return;

        Graphics::Texture* new_texture = new Graphics::Texture(_channels);
        new_texture->Create(_channels, _width, _height, _data, _settings);

        m_textures->insert({ _name, new_texture });
    }

    void InitStorage()
    {
        m_textures = new std::unordered_map<std::string, Graphics::Texture*>();
        m_objects = new std::unordered_map<std::string, Object3D*>();
    }

    void FreeStorage()
    {
        for(auto it = m_textures->begin(); it != m_textures->end(); ++it)
            delete it->second;
        
        for(auto it = m_objects->begin(); it != m_objects->end(); ++it)
            delete it->second;
        
        delete m_textures;
        delete m_objects;
    }
}