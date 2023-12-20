#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <list>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../utils/verticesArray.h"
#include "opengl/shader.h"
#include "opengl/opengl.h"
#include "../utils/verticesArray.h"
#include "camera.h"
#include "opengl/texture.h"
#include "opengl/frameBuffer.h"
#include "../storage/storage.hpp"
#include "../storage/object3d.hpp"

namespace Graphics
{
    class Renderer
    {
        private:
            /* opengl related */
            Graphics::Shader m_defaultShader;
            Graphics::Opengl m_opengl;
            Utils::VerticesArray m_verticesArray;

            unsigned int m_drawCallCount;

            /* camera */
            float m_nearValue, m_farValue;
            Graphics::Mat4Container m_cameraContainer;
            glm::mat4 m_projectionMatrix;

            Graphics::Camera3D m_defaultCamera;

            Graphics::Texture m_defaultTexture;
            
            /* basic rendering things */
            float m_setColor[4];

            float* m_polygonVertices;
            unsigned int* m_polygonIndices;

        public:
            Renderer(float _near, float _far);
            ~Renderer();

            void Init();

            void StartOfFrame();
            void EndOfFrame();

            Graphics::Camera3D* GetDefaultCamera() { return &m_defaultCamera; };

            void BindShader(Graphics::Shader& _shader);
            void BindFrameBuffer(Graphics::FrameBuffer& _frameBuffer);
            void BindDefaultFrameBuffer();
            void BindDefaultShader() { BindShader(m_defaultShader); };

            void Fill(float _brightness) { Fill(_brightness, _brightness, _brightness, 255); };
            void Fill(float _brightness, float _alpha) { Fill(_brightness, _brightness, _brightness, _alpha); };
            void Fill(float _red, float _green, float _blue) { Fill(_red, _green, _blue, 255); };
            void Fill(float _red, float _green, float _blue, float _alpha);

            void DrawRect(float _x, float _y, float _width, float _height) { DrawImage(m_defaultTexture, _x, _y, _width, _height); };
            void DrawImage(Graphics::Texture& _texture, float _x, float _y, float _width, float _height);
            void DrawPolygons(unsigned int _vertexSize, float* _vertices, unsigned int _indicesCount, unsigned int* _indices);
            void DrawSmallPolygons(unsigned int _vertexSize, float* _vertices, unsigned int _indicesCount, unsigned int* _indices);

            void RenderObject3DPolygon();

            unsigned int GetDrawCallCount() const { return m_drawCallCount; };

        private:
            void readFile(std::string& _output, const char* _path);
            void compileShader(unsigned int& _shader, const char* _shaderCode, unsigned int _shaderType);
            void createProgram(unsigned int& _shaderProgram, const char* _vertexPath, const char* _fragmentPath);
    };
}