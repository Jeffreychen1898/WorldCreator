#pragma once

#include <iostream>
#include <cmath>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/io.hpp>

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
            Camera3D();

            void Init(float _fov, float _aspectRatio, float _near, float _far);

            void SetPosition(float _x, float _y, float _z) { m_position = glm::vec3(_x, _y, _z); };
            void SetCenter(float _x, float _y, float _z) { m_center = glm::vec3(_x, _y, _z); };
            void SetUpDir(float _x, float _y, float _z) { m_upDir = glm::vec3(_x, _y, _z); };

            void MovePosition(float _dx, float _dy, float _dz) { m_position = m_position + glm::vec3(_dx, _dy, _dz); };
            void MoveCenter(float _dx, float _dy, float _dz) { m_center = m_center + glm::vec3(_dx, _dy, _dz); };
            void MoveForward(float _amount, bool _adjustCenter=true);
            void MoveRight(float _amount, bool _adjustCenter=true);
            void RotateVertical(float _angle); /* angle must be in the range (-90 deg, 90 deg) */
            void RotateHorizontal(float _angle); /* angle must be in the range (-90 deg, 90 deg) */

            void Update();

            void AttachShader(const char* _uniformName, Graphics::Shader& _shader) { _shader.AttachUniform(_uniformName, m_uniformContainer); };

            void PrintPosition() const { std::cout << m_position << std::endl; };
            void PrintCenter() const { std::cout << m_center << std::endl; };
    };
}