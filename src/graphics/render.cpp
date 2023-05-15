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

		m_verticesArray.SetVertexSize(m_defaultShader.GetVertexSize());

		/* setup the camera */
		float fov = glm::radians(45.f);
		float aspect_ratio = 1024.f / 768.f;
		m_defaultCamera.Init(fov, aspect_ratio, m_nearValue, m_farValue);

		m_defaultCamera.AttachShader("u_projection", m_defaultShader);

		m_defaultShader.Bind();

		/* texture */
		m_testTexture.Create();
		m_testTexture.Bind();

		m_defaultShader.GetUniformLocation("u_texture");
		float tex = 0;
		m_defaultShader.SetUniform("u_texture", &tex, UNIFORM_INT);
    }

    void Renderer::StartOfFrame()
    {
        m_opengl.ClearBuffers();
		m_verticesArray.Clear();
    }

	void Renderer::DrawPolygons(unsigned int _vertexSize, float* _vertices, unsigned int _indicesCount, unsigned int* _indices)
	{
		if(!m_verticesArray.AddShape(_vertexSize, _vertices, _indicesCount, _indices))
		{
			m_opengl.MakeDrawCall(m_defaultShader, m_verticesArray);
			m_verticesArray.AddShape(_vertexSize, _vertices, _indicesCount, _indices);
		}
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