#pragma once

#include <string>
#include <unordered_map>

#include "../graphics/opengl/texture.h"
#include "object3d.hpp"

// create a new plane:
//      create a plane with vertices
//      create a shift direction vector for each vertex
//      ->
//      create object3d to store the vertex

namespace Storage
{
    void InitStorage();
    void FreeStorage();

    Object3D* GetObject(std::string& _name);
    Graphics::Texture* GetTexture(std::string& _name);

    void CreatePlane(std::string& _name, float _width, float _height,
    unsigned int _sectionX, unsigned int _sectionY);

    void CreateTexture(std::string& _name, unsigned int _channels, unsigned int _width,
        unsigned int _height, unsigned char* _data, unsigned char _settings);
}