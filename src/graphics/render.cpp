#include "render.h"

namespace Graphics
{
    Renderer::Renderer(float _near, float _far)
		: m_nearValue(_near), m_farValue(_far)
    {
    }

    void Renderer::Init()
    {
		m_opengl.Init();

		m_defaultShader.Create(6, "res/shaders/default/vertex.glsl", "res/shaders/default/fragment.glsl");
		m_defaultShader.AddVertexBuffer(1, 3, 0);
		m_defaultShader.AddVertexBuffer(2, 3, 3);

		/* setup the camera */
		float fovy = glm::radians(90.f);
		float aspect_ratio = 1024.f / 768.f;
		m_projectionMatrix = glm::perspective(fovy, aspect_ratio, m_nearValue, m_farValue);
		glm::mat4 default_view = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));

		glm::mat4 mvp_matrix = m_projectionMatrix * default_view;

		m_cameraContainer.SetData(&mvp_matrix[0][0]);

		m_defaultShader.AttachUniform("u_projection", m_cameraContainer);

		m_verticesArray.SetVertexSize(m_defaultShader.GetVertexSize());
    }

    void Renderer::StartOfFrame()
    {
        m_opengl.ClearBuffers();
		m_verticesArray.Clear();
    }

	void Renderer::DrawRect(float x, float y, float width, float height)
	{
		float data[] = {
    		x, y, -30.0,				1.0, 0.0, 0.0,
    		x+width, y, -30.0,			0.0, 1.0, 0.0,
    		x+width,  y+height, -30.0,	1.0, 1.0, 1.0,
	    	x,  y+height, -30.0,		1.0, 1.0, 0.0
    	};
    	unsigned int indices[] = {
    		0, 1, 2,
    		0, 2, 3
    	};

		if(!m_verticesArray.AddShape(24, data, 6, indices))
		{
			m_opengl.MakeDrawCall(m_defaultShader, m_verticesArray);
			m_verticesArray.AddShape(24, data, 6, indices);
		}
	}

    void Renderer::EndOfFrame()
    {
		m_opengl.MakeDrawCall(m_defaultShader, m_verticesArray);
    }

    Renderer::~Renderer()
    {
    }
}