#include "opengl.h"

namespace Graphics
{
    Opengl::Opengl()
    {
        //
    }

    void Opengl::Init()
    {
        //
    }

    void Opengl::ClearBuffers()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Opengl::MakeDrawCall(Graphics::Shader& _shaderProgram, Utils::VerticesArray& _verticesArray)
    {
        _shaderProgram.Bind();

        unsigned int vertex_count = (unsigned int)(_verticesArray.GetVertexSize() / _shaderProgram.GetVertexSize());
        _shaderProgram.AttributeData(vertex_count, _verticesArray.GetVerticesData());
		_shaderProgram.IndexBufferData(_verticesArray.GetIndicesSize(), _verticesArray.GetIndicesData());

        glDrawElements(GL_TRIANGLES, _verticesArray.GetIndicesSize(), GL_UNSIGNED_INT, 0);
        _verticesArray.Clear();

        /* call bind to update all of the uniforms that have not been updated */
        _shaderProgram.Bind();
    }

    Opengl::~Opengl()
    {
        //
    }
}