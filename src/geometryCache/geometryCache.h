#pragma once

#include <unordered_map>

#include "shape.h"
#include "../graphics/render.h"

namespace GeometryCache
{
    class ShapesCache
    {
        private:
            std::unordered_map<std::string, GeometryCache::Shapes*>* m_shapes;

        public:
            ShapesCache();
            ~ShapesCache();

            bool AppendShape(const char* _name, unsigned int _vertexSize, float* _vertices, unsigned int _indicesCount, unsigned int* _indices);
            void RemoveShape(const char* _name);
            bool Rename(const char* _name, const char* _newname);

            void RenderShapes(Graphics::Renderer& _renderer);

            bool AppendPlane(const char* _name, float _x, float _y, float _z);

            void SetShapeRotation(const char* _name, float _x, float _y, float _z);
    };
}