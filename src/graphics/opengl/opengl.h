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
            void MakeDrawCall(Utils::VerticesArray& _verticesArray, unsigned int& _drawCallCount);
    };
}