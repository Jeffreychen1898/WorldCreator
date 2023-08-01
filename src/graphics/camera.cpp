#include "camera.h"

namespace Graphics
{
    Camera3D::Camera3D()
    {
        m_projection = glm::perspective(glm::radians(90.f), 1.f, 1.f, 10.f);

        m_position = glm::vec3(0.f, 0.f, 0.f);
        m_center = glm::vec3(0.f, 0.f, -1.f);
        m_upDir = glm::vec3(0.f, 1.f, 0.f);

        Update();
    }

    void Camera3D::Init(float _fov, float _aspectRatio, float _near, float _far)
    {
        m_projection = glm::perspective(_fov, _aspectRatio, _near, _far);

        Update();
    }

    void Camera3D::Update()
    {
        glm::mat4 default_view = glm::lookAt(m_position, m_center, m_upDir);
        glm::mat4 camera_matrix = m_projection * default_view;

        m_uniformContainer.SetData(&camera_matrix[0][0]);
    }

    void Camera3D::RotateCenterVertical(float _angle)
    {
        glm::vec3 center_direction = m_center - m_position;
        glm::vec3 direction = glm::normalize(-1.f * m_upDir);

        /* calculate how much the target moves in the vertical direction */
        float radius = length(center_direction);
        glm::vec3 delta_center = (float)(radius * tan(-_angle)) * direction;
        /* make the center at the same radius from the camera */
        delta_center = glm::normalize(delta_center + center_direction) * radius;

        m_center = delta_center + m_position;
    }

    void Camera3D::RotateCenterHorizontal(float _angle)
    {
        glm::vec3 center_direction = m_center - m_position;
        glm::vec3 direction = glm::normalize(glm::cross(center_direction, m_upDir));

        /* calculate how much the target moves in the horizontal direction */
        float radius = glm::length(center_direction);
        glm::vec3 delta_center = (float)(radius * tan(-_angle)) * direction;
        /* make the center at the same radius from the camera */
        delta_center = glm::normalize(delta_center + center_direction) * radius;
        
        m_center = delta_center + m_position;
    }

    void Camera3D::RotateVertical(float _angle)
    {
        glm::vec3 center_direction = m_position - m_center;
        glm::vec3 direction = glm::normalize(-1.f * m_upDir);

        /* calculate how much the target moves in the vertical direction */
        float radius = length(center_direction);
        glm::vec3 delta_position = (float)(radius * tan(-_angle)) * direction;
        /* make the center at the same radius from the camera */
        delta_position = glm::normalize(delta_position + center_direction) * radius;

        m_position = delta_position + m_center;
    }

    void Camera3D::RotateHorizontal(float _angle)
    {
        glm::vec3 center_direction = m_position - m_center;
        glm::vec3 direction = glm::normalize(glm::cross(center_direction, m_upDir));

        /* calculate how much the target moves in the horizontal direction */
        float radius = glm::length(center_direction);
        glm::vec3 delta_position = (float)(radius * tan(_angle)) * direction;
        /* make the center at the same radius from the camera */
        delta_position = glm::normalize(delta_position + center_direction) * radius;
        
        m_position = delta_position + m_center;
    }

    void Camera3D::MoveForward(float _amount, bool _adjustCenter)
    {
        glm::vec3 forward_direction = glm::normalize(m_center - m_position);
        m_position = m_position + (_amount * forward_direction);
        if(_adjustCenter)
            m_center = m_center + (_amount * forward_direction);
    }

    void Camera3D::MoveForwardXZPlane(float _amount, bool _adjustCenter)
    {
        glm::vec3 right_direction = glm::cross(m_center - m_position, m_upDir);
        glm::vec3 forward_direction = glm::normalize(glm::cross(m_upDir, right_direction));
        m_position = m_position + (_amount * forward_direction);
        if(_adjustCenter)
            m_center = m_center + (_amount * forward_direction);
    }

    void Camera3D::MoveRight(float _amount, bool _adjustCenter)
    {
        glm::vec3 right_direction = glm::normalize(glm::cross(m_center - m_position, m_upDir));
        m_position = m_position + (_amount * right_direction);
        if(_adjustCenter)
            m_center = m_center + (_amount * right_direction);
    }

    double Camera3D::GetAngle(float _dirX, float _dirY, float _dirZ) const
    {
        glm::vec3 dir_vector = glm::normalize(glm::vec3(_dirX, _dirY, _dirZ));
        glm::vec3 camera_dir = glm::normalize(m_center - m_position);

        return acos(glm::dot(camera_dir, dir_vector));
    }
}