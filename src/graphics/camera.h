#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "opengl/shader.h"

namespace Graphics
{
    class Camera3D
    {
        private:
            glm::mat4 m_projection;

            glm::vec3 m_position;
            glm::vec3 m_center;
            glm::vec3 m_upDir;

            Graphics::Mat4Container m_uniformContainer;
        public:
            Camera3D()
            {
                m_projection = glm::perspective(glm::radians(90.f), 1.f, 1.f, 10.f);

                m_position = glm::vec3(0.f, 0.f, 0.f);
                m_center = glm::vec3(0.f, 0.f, -1.f);
                m_upDir = glm::vec3(0.f, 1.f, 0.f);

                Update();
            }

            void Init(float _fov, float _aspectRatio, float _near, float _far)
            {
                m_projection = glm::perspective(_fov, _aspectRatio, _near, _far);

                Update();
            };

            void SetPosition(float _x, float _y, float _z) { m_position = glm::vec3(_x, _y, _z); };
            void SetCenter(float _x, float _y, float _z) { m_center = glm::vec3(_x, _y, _z); };
            void SetUpDir(float _x, float _y, float _z) { m_upDir = glm::vec3(_x, _y, _z); };

            void MovePosition(float _dx, float _dy, float _dz) { m_position = m_position + glm::vec3(_dx, _dy, _dz); };
            void MoveCenter(float _dx, float _dy, float _dz) { m_center = m_center + glm::vec3(_dx, _dy, _dz); };

            void Update()
            {
                glm::mat4 default_view = glm::lookAt(m_position, m_center, m_upDir);
                glm::mat4 camera_matrix = m_projection * default_view;

                m_uniformContainer.SetData(&camera_matrix[0][0]);
            };

            void AttachShader(const char* _uniformName, Graphics::Shader& _shader) { _shader.AttachUniform(_uniformName, m_uniformContainer); };
    };
}