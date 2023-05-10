#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "opengl/shader.h"
#include "opengl/opengl.h"
#include "../utils/verticesArray.h"

namespace Graphics
{
    class Renderer
    {
        private:
            /* opengl related */
            Graphics::Shader m_defaultShader;
            Graphics::Opengl m_opengl;
            Utils::VerticesArray m_verticesArray;

            /* camera */
            float m_nearValue, m_farValue;
            Graphics::Mat4Container m_cameraContainer;
            glm::mat4 m_projectionMatrix;

        public:
            Renderer(float _near, float _far);
            ~Renderer();

            void Init();

            void StartOfFrame();
            void EndOfFrame();

            /* temporary functions */
            void DrawRect(float x, float y, float width, float height);

        private:
            void readFile(std::string& _output, const char* _path);
            void compileShader(unsigned int& _shader, const char* _shaderCode, unsigned int _shaderType);
            void createProgram(unsigned int& _shaderProgram, const char* _vertexPath, const char* _fragmentPath);
    };
}