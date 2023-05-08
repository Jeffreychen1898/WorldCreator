#include "render.h"

namespace Graphics
{
    Renderer::Renderer()
    {
    }

    void Renderer::Init()
    {
		m_defaultShader.Create(6, "res/shaders/default/vertex.glsl", "res/shaders/default/fragment.glsl");
		m_defaultShader.AddVertexBuffer(1, 3, 0);
		m_defaultShader.AddVertexBuffer(2, 3, 3);
    }

    void Renderer::StartOfFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        float data[] = {
    		-0.5, -0.5, 0.0,	1.0, 0.0, 0.0,
    		 0.5, -0.5, 0.0,	0.0, 1.0, 0.0,
    		 0.5,  0.5, 0.0,	1.0, 1.0, 1.0,
	    	-0.5,  0.5, 0.0,	1.0, 1.0, 0.0
    	};
    	unsigned int indices[] = {
    		0, 1, 2,
    		0, 2, 3
    	};
		m_defaultShader.AttributeData(4, data);
		m_defaultShader.IndexBufferData(6, indices);
    }

    void Renderer::EndOfFrame()
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    Renderer::~Renderer()
    {
    }
}