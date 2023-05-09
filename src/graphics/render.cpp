#include "render.h"

namespace Graphics
{
    Renderer::Renderer()
    {
    }

    void Renderer::Init()
    {
		m_opengl.Init();

		m_defaultShader.Create(6, "res/shaders/default/vertex.glsl", "res/shaders/default/fragment.glsl");
		m_defaultShader.AddVertexBuffer(1, 3, 0);
		m_defaultShader.AddVertexBuffer(2, 3, 3);

		glm::mat4 proj = glm::ortho(0.0, 1024.0, 768.0, 0.0, -1.0, 1.0);

		m_projectionMatrix.SetData(&proj[0][0]);

		m_defaultShader.AttachUniform("u_projection", m_projectionMatrix);

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
    		x, y, 0.0,					1.0, 0.0, 0.0,
    		x+width, y, 0.0,			0.0, 1.0, 0.0,
    		x+width,  y+height, 0.0,	1.0, 1.0, 1.0,
	    	x,  y+height, 0.0,			1.0, 1.0, 0.0
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