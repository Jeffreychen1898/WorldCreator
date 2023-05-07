#include "render.h"

namespace Graphics
{
    Renderer::Renderer()
    {
        //
    }

    void Renderer::Init()
    {
        createProgram(m_shaderProgram, "res/shaders/default/vertex.glsl", "res/shaders/default/fragment.glsl");

	    glGenVertexArrays(1, &m_vao);
    	glBindVertexArray(m_vao);

    	glGenBuffers(1, &m_positionVbo);
    	glBindBuffer(GL_ARRAY_BUFFER, m_positionVbo);

    	glGenBuffers(1, &m_colorVbo);
    	glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);

        glBindBuffer(GL_ARRAY_BUFFER, m_positionVbo);
    	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    	glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
    	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    	glEnableVertexAttribArray(2);

	    unsigned int ibo;
    	glGenBuffers(1, &ibo);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    }

    void Renderer::StartOfFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        float data[] = {
    		-0.5, -0.5, 0.0,	1.0, 0.0, 0.0,
    		 0.5, -0.5, 0.0,	0.0, 1.0, 0.0,
    		 0.5,  0.5, 0.0,	0.0, 0.0, 1.0,
	    	-0.5,  0.5, 0.0,	1.0, 1.0, 0.0
    	};
    	int indices[] = {
    		0, 1, 2,
    		0, 2, 3
    	};
        glBindBuffer(GL_ARRAY_BUFFER, m_positionVbo);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void Renderer::EndOfFrame()
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void Renderer::readFile(std::string& _output, const char* _path)
    {
        std::ifstream read_file(_path);

	    std::string line;
        _output = "";

	    if(read_file.is_open())
	    {
		    while(std::getline(read_file, line))
	    		_output += line + "\n";

	    	read_file.close();
    	} else
            std::cout << "[Error] unable to open file: " << _path << std::endl;
    }

    void Renderer::compileShader(unsigned int& _shader, const char* _shaderCode, unsigned int _shaderType)
    {
        _shader = glCreateShader(_shaderType);
	    glShaderSource(_shader, 1, &_shaderCode, nullptr);
	    glCompileShader(_shader);

	    #if DEBUG_MODE
    	int result;
    	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result);
    	if(result == GL_FALSE)
    	{
    		int length;
    		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &length);
    		char* message = new char[length];
    		glGetShaderInfoLog(_shader, length, &length, message);
    		const char* shader_type = (_shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
    		std::cout << "[Error] Failed to compile " << shader_type << " Shader!" << std::endl;
    		std::cout << message << std::endl;
    		glDeleteShader(_shader);
    	}
    	#endif
    }

    void Renderer::createProgram(unsigned int& _shaderProgram, const char* _vertexPath, const char* _fragmentPath)
    {
        _shaderProgram = glCreateProgram();

        std::string vertex_code, fragment_code;
        readFile(vertex_code, _vertexPath);
        readFile(fragment_code, _fragmentPath);

        unsigned int vertex_shader, fragment_shader;
    	compileShader(vertex_shader, vertex_code.c_str(), GL_VERTEX_SHADER);
    	compileShader(fragment_shader, fragment_code.c_str(), GL_FRAGMENT_SHADER);

    	glAttachShader(_shaderProgram, vertex_shader);
    	glAttachShader(_shaderProgram, fragment_shader);
    	glLinkProgram(_shaderProgram);
    	glValidateProgram(_shaderProgram);

    	glDeleteShader(vertex_shader);
    	glDeleteShader(fragment_shader);

    	glUseProgram(_shaderProgram);
    }

    Renderer::~Renderer()
    {
        glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_positionVbo);
		glDeleteBuffers(1, &m_colorVbo);
		glDeleteBuffers(1, &m_ibo);
		glDeleteProgram(m_shaderProgram);
    }
}