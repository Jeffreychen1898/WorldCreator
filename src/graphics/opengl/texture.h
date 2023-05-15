#pragma once

#include <glad.h>
#include <GLFW/glfw3.h>

namespace Graphics {
    class Texture {
        private:
            unsigned int m_textureId;

        public:
            Texture();
            ~Texture();

            void Create();
            void Bind();
    };
}