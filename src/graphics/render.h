#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "opengl/shader.h"

namespace Graphics
{
    class Renderer
    {
        private:
            Graphics::Shader m_defaultShader;

        public:
            Renderer();
            ~Renderer();

            void Init();

            void StartOfFrame();
            void EndOfFrame();

        private:
            void readFile(std::string& _output, const char* _path);
            void compileShader(unsigned int& _shader, const char* _shaderCode, unsigned int _shaderType);
            void createProgram(unsigned int& _shaderProgram, const char* _vertexPath, const char* _fragmentPath);
    };
}