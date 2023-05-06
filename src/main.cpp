#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

static unsigned int compileShader(unsigned int shaderType, const char* source)
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	#if DEBUG_MODE
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(shader, length, &length, message);
		const char* shader_type = (shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
		std::cout << "Failed to compile " << shader_type << " Shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(shader);
	}
	#endif

	return shader;
}

unsigned int createShader(const char* vertex, const char* fragment)
{
	unsigned int program = glCreateProgram();
	unsigned int vertex_shader;
	unsigned int fragment_shader;

	vertex_shader = compileShader(GL_VERTEX_SHADER, vertex);
	fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragment);
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glUseProgram(program);

	return program;
}

int main()
{
	const char* vertCode =
	"#version 330 core\n"
	"layout(location=1) in vec3 a_position;\n"
	"void main()\n"
	"{\n"
		"gl_Position = vec4(a_position, 1.0);\n"
	"}\0";

	const char* fragCode =
	"#version 330 core\n"
	"void main()\n"
	"{\n"
		"gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);\n"
	"}\0";

	#if DEBUG_MODE
	std::cout << "Debug Mode On" << std::endl;
	#endif

	GLFWwindow* window;

	if(!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	unsigned int shaderProgram = createShader(vertCode, fragCode);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float data[] = {
		-0.5, -0.5, 0.0,
		 0.5, -0.5, 0.0,
		 0.0,  0.5, 0.0
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	while(!glfwWindowShouldClose(window))
	{
		//glClearColor(1.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
