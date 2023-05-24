#include "render.h"

namespace Graphics
{
    Renderer::Renderer(float _near, float _far)
		: m_nearValue(_near), m_farValue(_far), m_defaultTexture(4), m_drawCallCount(0)
    {
		m_setColor[0] = 0;
		m_setColor[1] = 0;
		m_setColor[2] = 0;
		m_setColor[3] = 0;
    }

    void Renderer::Init()
    {
		m_opengl.Init();

		/* setup the default shader */
		m_defaultShader.Create(9, "res/shaders/default/vertex.glsl", "res/shaders/default/fragment.glsl");
		m_defaultShader.AddVertexBuffer(1, 3, 0);
		m_defaultShader.AddVertexBuffer(2, 4, 3);
		m_defaultShader.AddVertexBuffer(3, 2, 7);

		m_verticesArray.SetVertexSize(m_defaultShader.GetVertexSize());

		/* setup the camera */
		float fov = glm::radians(45.f);
		float aspect_ratio = 1024.f / 768.f;
		m_defaultCamera.Init(fov, aspect_ratio, m_nearValue, m_farValue);

		m_defaultCamera.AttachShader("u_projection", m_defaultShader);

		m_defaultShader.Bind();

		/* setup the default texture */
		float texture_slot = 0;
		unsigned char white_color[] = {255, 255, 255, 255};
		m_defaultTexture.Create(4, 1, 1, white_color);
		m_defaultTexture.Bind(0);

		m_defaultShader.GetUniformLocation("u_texture");
		m_defaultShader.SetUniform("u_texture", &texture_slot, UNIFORM_INT);
    }

    void Renderer::StartOfFrame()
    {
        m_opengl.ClearBuffers();
		m_verticesArray.Clear();

		m_drawCallCount = 0;
    }

	void Renderer::BindShader(Graphics::Shader& _shader)
	{
		if(!_shader.IsBounded())
		{
			if(Graphics::Shader::s_currentShaderPtr != nullptr)
				m_opengl.MakeDrawCall(m_verticesArray, m_drawCallCount);
			
			_shader.Bind();
			m_verticesArray.SetVertexSize(_shader.GetVertexSize());
		}
	}

	void Renderer::BindFrameBuffer(Graphics::FrameBuffer& _frameBuffer)
	{
		if(Graphics::FrameBuffer::s_currentFrameBuffer == _frameBuffer.GetId())
			return;

			m_opengl.MakeDrawCall(m_verticesArray, m_drawCallCount);

			_frameBuffer.Bind();
	}

    void Renderer::BindDefaultFrameBuffer()
	{
		if(Graphics::FrameBuffer::s_currentFrameBuffer == 0)
			return;
		
		m_opengl.MakeDrawCall(m_verticesArray, m_drawCallCount);

		Graphics::FrameBuffer::BindDefault();
	}

	void Renderer::DrawPolygons(unsigned int _vertexSize, float* _vertices, unsigned int _indicesCount, unsigned int* _indices)
	{
		if(_indicesCount < MAX_INDICES && _vertexSize < MAX_VERTEX_SIZE)
		{
			if(!m_verticesArray.AddShape(_vertexSize, _vertices, _indicesCount, _indices))
			{
				m_opengl.MakeDrawCall(m_verticesArray, m_drawCallCount);
				m_verticesArray.AddShape(_vertexSize, _vertices, _indicesCount, _indices);
			}
			return;
		}

		// split into multiple batches
	}

	void Renderer::Fill(float _red, float _green, float _blue, float _alpha)
	{
		m_setColor[0] = _red / 255.f;
		m_setColor[1] = _green / 255.f;
		m_setColor[2] = _blue / 255.f;
		m_setColor[3] = _alpha / 255.f;
	}

	void Renderer::DrawImage(Graphics::Texture& _texture, float _x, float _y, float _width, float _height)
	{
		BindShader(m_defaultShader);
		_texture.Bind(0);

		float data[] = {
    		_x, _y, 0.0,					m_setColor[0], m_setColor[1], m_setColor[2], m_setColor[3],			0.0, 1.0,
    		_x+_width, _y, 0.0,				m_setColor[0], m_setColor[1], m_setColor[2], m_setColor[3],			1.0, 1.0,
    		_x+_width,  _y+_height, 0.0,	m_setColor[0], m_setColor[1], m_setColor[2], m_setColor[3],			1.0, 0.0,
	    	_x,  _y+_height, 0.0,			m_setColor[0], m_setColor[1], m_setColor[2], m_setColor[3],			0.0, 0.0
    	};
    	unsigned int indices[] = {
    		0, 1, 2,
    		0, 2, 3
    	};

		if(!m_verticesArray.AddShape(36, data, 6, indices))
		{
			m_opengl.MakeDrawCall(m_verticesArray, m_drawCallCount);
			m_verticesArray.AddShape(36, data, 6, indices);
		}
	}

    void Renderer::EndOfFrame()
    {
		m_opengl.MakeDrawCall(m_verticesArray, m_drawCallCount);
    }

    Renderer::~Renderer()
    {
    }
}