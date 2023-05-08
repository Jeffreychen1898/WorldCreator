#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include <glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
    class Shader
    {
        private:
            unsigned int m_shaderProgram;
            unsigned int m_vertexArray;
            unsigned int m_indexBuffer;
            unsigned int m_vertexSize;

            /* vertex buffer location -> vbo */
            std::unordered_map<unsigned int, unsigned int> m_vertexBuffers;

        public:
            Shader();
            ~Shader();

            void Create(unsigned int _vertexSize, const char* _vertexPath, const char* _fragmentPath);
            void AddVertexBuffer(unsigned int _location, unsigned int _size, unsigned int _offset);
            void AttributeData(unsigned int _vertexCount, float* data);
            void IndexBufferData(unsigned int size, unsigned int* data);

            void Bind();

        private:
            void CompileShader(unsigned int& _shader, const char* _shaderCode, unsigned int _shaderType);
            void CreateProgram(unsigned int& _program, const char* _vertexCode, const char* _fragmentCode);
            void readFile(std::string& _output, const char* _filename);
    };
}