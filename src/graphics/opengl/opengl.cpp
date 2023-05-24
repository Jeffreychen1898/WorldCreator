#include "opengl.h"

namespace Graphics
{
    Opengl::Opengl()
    {
        //
    }

    void Opengl::Init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Opengl::ClearBuffers()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Graphics::FrameBuffer::s_currentFrameBuffer = 0;
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Opengl::MakeDrawCall(Utils::VerticesArray& _verticesArray, unsigned int& _drawCallCount)
    {
        if(_verticesArray.IsEmpty())
            return;

        ++ _drawCallCount;
        Graphics::Shader* shader_program = Graphics::Shader::s_currentShaderPtr;
        unsigned int vertex_count = (unsigned int)(_verticesArray.GetVertexSize() / shader_program->GetVertexSize());
        shader_program->AttributeData(vertex_count, _verticesArray.GetVerticesData());
		shader_program->IndexBufferData(_verticesArray.GetIndicesSize(), _verticesArray.GetIndicesData());

        glDrawElements(GL_TRIANGLES, _verticesArray.GetIndicesSize(), GL_UNSIGNED_INT, 0);
        _verticesArray.Clear();

        /* call bind to update all of the uniforms that have not been updated */
        shader_program->Bind();
    }

    Opengl::~Opengl()
    {
        //
    }
}