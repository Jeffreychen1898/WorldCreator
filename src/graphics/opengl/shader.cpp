#include "shader.h"

namespace Graphics
{
    Graphics::Shader* Graphics::Shader::s_currentShaderPtr = nullptr;
    Shader::Shader()
        : m_shaderProgram(0), m_vertexArray(0), m_indexBuffer(0)
    {
    }

    void Shader::Create(unsigned int _vertexSize, const char* _vertexPath, const char* _fragmentPath)
    {
        /* create the shader program */
        std::string vertex_shader_code, fragment_shader_code;
        readFile(vertex_shader_code, _vertexPath);
        readFile(fragment_shader_code, _fragmentPath);

        CreateProgram(m_shaderProgram, vertex_shader_code.c_str(), fragment_shader_code.c_str());

        /* create the vao */
        glGenVertexArrays(1, &m_vertexArray);
    	glBindVertexArray(m_vertexArray);

        /* create the ibo */
        glGenBuffers(1, &m_indexBuffer);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

        m_vertexSize = _vertexSize;

        s_currentShaderPtr = this;
    }

    void Shader::AddVertexBuffer(unsigned int _location, unsigned int _size, unsigned int _offset)
    {
        Bind();

        unsigned int vertex_buffer;
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    	glVertexAttribPointer(_location, _size, GL_FLOAT, GL_FALSE, m_vertexSize * sizeof(float), (void*)(_offset*sizeof(float)));
    	glEnableVertexAttribArray(_location);

        m_vertexBuffers.insert({ _location, vertex_buffer });
    }

    void Shader::AttachUniform(const char* _uniformName, Graphics::ShaderUniformContainer& _uniformContainer)
    {
        GetUniformLocation(_uniformName);

        SetUniform(_uniformName, _uniformContainer.GetData(), _uniformContainer.GetUniformType());

        _uniformContainer.BindShader(m_shaderProgram);
        m_uniformList.push_back({ _uniformName, &_uniformContainer });
    }

    void Shader::GetUniformLocation(const char* _uniformName)
    {
        Bind();

        unsigned int get_uniform_location = glGetUniformLocation(m_shaderProgram, _uniformName);
        m_uniformLocations.insert({ _uniformName, get_uniform_location });
    }

    void Shader::AttributeData(unsigned int _vertexCount, float* _data)
    {
        Bind();

        for(const auto& pair : m_vertexBuffers)
        {
            glBindBuffer(GL_ARRAY_BUFFER, pair.second);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexCount * m_vertexSize, _data, GL_DYNAMIC_DRAW);
        }
    }

    void Shader::IndexBufferData(unsigned int size, unsigned int* _data)
    {
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * size, _data, GL_DYNAMIC_DRAW);
    }

    void Shader::SetUniform(const char* _uniformName, float* _data, unsigned int _uniformType)
    {
        unsigned int uniform_location = m_uniformLocations[_uniformName];
        switch(_uniformType)
        {
            case UNIFORM_MAT4:
                glUniformMatrix4fv(uniform_location, 1, GL_FALSE, _data);
                break;
            case UNIFORM_INT:
                glUniform1i(uniform_location, (int)(*_data));
                break;
            case UNIFORM_VEC4:
                glUniform4f(uniform_location, *_data, *(_data + 1), *(_data + 2), *(_data + 3));
                break;
            case UNIFORM_VEC3:
                glUniform3f(uniform_location, *_data, *(_data + 1), *(_data + 2));
                break;
            case UNIFORM_VEC2:
                glUniform2f(uniform_location, *_data, *(_data + 1));
                break;
            case UNIFORM_FLOAT:
                glUniform1f(uniform_location, *_data);
                break;
        }
    }

    void Shader::Bind(bool _updateUniforms)
    {
        bool shader_will_bind = false;
        if(s_currentShaderPtr != this)
        {
            glUseProgram(m_shaderProgram);
            glBindVertexArray(m_vertexArray);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

            s_currentShaderPtr = this;

            shader_will_bind = true;
        }

        if(!shader_will_bind && !_updateUniforms)
            return;

        for(const auto& each_uniform : m_uniformList)
        {
            if(!each_uniform.second->IsShaderUpdated(m_shaderProgram))
            {
                SetUniform(each_uniform.first, each_uniform.second->GetData(), each_uniform.second->GetUniformType());
                each_uniform.second->SetShaderUpdated(m_shaderProgram);
            }
        }
    }

    bool Shader::IsBounded() const
    {
        if(s_currentShaderPtr == nullptr)
            return false;

        return s_currentShaderPtr == this;
    }

    void Shader::CompileShader(unsigned int& _shader, const char* _shaderCode, unsigned int _shaderType)
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

    void Shader::CreateProgram(unsigned int& _program, const char* _vertexCode, const char* _fragmentCode)
    {
        _program = glCreateProgram();

        unsigned int vertex_shader, fragment_shader;
    	CompileShader(vertex_shader, _vertexCode, GL_VERTEX_SHADER);
    	CompileShader(fragment_shader, _fragmentCode, GL_FRAGMENT_SHADER);

    	glAttachShader(_program, vertex_shader);
    	glAttachShader(_program, fragment_shader);
    	glLinkProgram(_program);
    	glValidateProgram(_program);

    	glDeleteShader(vertex_shader);
    	glDeleteShader(fragment_shader);

    	glUseProgram(_program);
    }

    void Shader::readFile(std::string& _output, const char* _filename)
    {
        std::ifstream read_file(_filename);

	    std::string line;
        _output = "";

	    if(read_file.is_open())
	    {
		    while(std::getline(read_file, line))
	    		_output += line + "\n";

	    	read_file.close();
    	} else
            std::cout << "[Error] unable to open file: " << _filename << std::endl;
    }

    Shader::~Shader()
    {
        glDeleteVertexArrays(1, &m_vertexArray);
        glDeleteBuffers(1, &m_indexBuffer);
        glDeleteProgram(m_shaderProgram);

        for(const auto& pair : m_vertexBuffers)
            glDeleteBuffers(1, &pair.second);

        if(s_currentShaderPtr == this)
            s_currentShaderPtr = nullptr;
    }

    bool ShaderUniformContainer::IsShaderUpdated(unsigned int _shaderId)
    {
        if(m_shaderUpdated.find(_shaderId) != m_shaderUpdated.end())
            return m_shaderUpdated[_shaderId];
        
        return true;
    }

    void ShaderUniformContainer::SetAllNotUpdated()
    {
        for(auto& [key, value] : m_shaderUpdated)
            value = false;
    }

    Mat4Container::Mat4Container()
        : ShaderUniformContainer(UNIFORM_MAT4)
    {
        for(unsigned int i=0;i<16;++i)
            m_data[i] = (i % 5 == 0) ? 1 : 0;
    }

    FloatContainer::FloatContainer()
        : ShaderUniformContainer(UNIFORM_FLOAT)
    {
        m_data[0] = 0.f;
    }

    Vec2Container::Vec2Container()
        : ShaderUniformContainer(UNIFORM_VEC2)
    {
        for(unsigned int i=0;i<2;++i)
            m_data[i] = 0.f;
    }

    Vec3Container::Vec3Container()
        : ShaderUniformContainer(UNIFORM_VEC3)
    {
        for(unsigned int i=0;i<3;++i)
            m_data[i] = 0.f;
    }

    Vec4Container::Vec4Container()
        : ShaderUniformContainer(UNIFORM_VEC4)
    {
        for(unsigned int i=0;i<4;++i)
            m_data[i] = 0.f;
    }
}