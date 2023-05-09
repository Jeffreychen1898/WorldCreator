#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <cstring>

#include <glad.h>
#include <GLFW/glfw3.h>

#define UNIFORM_MAT4 0

namespace Graphics
{
    class ShaderUniformContainer
    {
        private:
            /* shader program -> shader uniform updated */
            std::unordered_map<unsigned int, bool> m_shaderUpdated;
            unsigned int m_uniformType;

        public:
            ShaderUniformContainer(unsigned int _uniformType) { m_uniformType = _uniformType; };

            virtual float* GetData() = 0;

            void BindShader(unsigned int _shaderId) { m_shaderUpdated.insert({ _shaderId, false }); };
            void SetShaderUpdated(unsigned int _shaderId) { m_shaderUpdated[_shaderId] = true; };

            unsigned int GetUniformType() const { return m_uniformType; };
            bool IsShaderUpdated(unsigned int _shaderId);
    };

    class Mat4Container : public ShaderUniformContainer
    {
        private:
            float m_data[16];

        public:
            Mat4Container();

            float* GetData() { return m_data; };

            void SetData(float* _data) { memcpy(m_data, _data, sizeof(float) * 16); };
    };

    class Shader
    {
        private:
            unsigned int m_shaderProgram;
            unsigned int m_vertexArray;
            unsigned int m_indexBuffer;
            unsigned int m_vertexSize;

            /* vertex buffer location -> vbo */
            std::unordered_map<unsigned int, unsigned int> m_vertexBuffers;

            /* uniform name -> uniform location */
            std::unordered_map<const char*, unsigned int> m_uniformLocations;

            /* uniform name & uniform container */
            std::list<std::pair<const char*, Graphics::ShaderUniformContainer*>> m_uniformList;

        public:
            Shader();
            ~Shader();

            void Create(unsigned int _vertexSize, const char* _vertexPath, const char* _fragmentPath);

            void AddVertexBuffer(unsigned int _location, unsigned int _size, unsigned int _offset);
            void AttachUniform(const char* _uniformName, Graphics::ShaderUniformContainer& _uniformContainer);

            void AttributeData(unsigned int _vertexCount, float* _data);
            void IndexBufferData(unsigned int size, unsigned int* _data);

            void Bind();

            unsigned int GetVertexSize() const { return m_vertexSize; };

        private:
            void CompileShader(unsigned int& _shader, const char* _shaderCode, unsigned int _shaderType);
            void CreateProgram(unsigned int& _program, const char* _vertexCode, const char* _fragmentCode);
            void readFile(std::string& _output, const char* _filename);

            void SetUniform(const char* _uniformName, float* _data, unsigned int _uniformType);
    };
}