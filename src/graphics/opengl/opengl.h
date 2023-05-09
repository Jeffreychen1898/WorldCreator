#pragma once

#include <glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "../../utils/verticesArray.h"

namespace Graphics
{
    class Opengl
    {
        private:
            //
        public:
            Opengl();
            ~Opengl();
            void Init();

            void ClearBuffers();
            void MakeDrawCall(Graphics::Shader& _shaderProgram, Utils::VerticesArray& _verticesArray);
    };
}