#include <iostream>
#include <fstream>

#include <glad.h>
#include <GLFW/glfw3.h>

static std::string readFile(const char* filePath)
{
	std::ifstream inputFile(filePath);
	std::string line;

	std::string result = "";

	if(inputFile.is_open())
	{
		bool first_line = true;
		while(std::getline(inputFile, line))
		{
			if(!first_line)
				result += "\n";
			else
				first_line = false;
			result += line;
		}

		inputFile.close();
	} else {
		std::cout << "unable to open file" << std::endl;
	}

	return result;
}

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


	std::string vertex_code = readFile(vertex).c_str();
	std::string fragment_code = readFile(fragment).c_str();

	vertex_shader = compileShader(GL_VERTEX_SHADER, vertex_code.c_str());
	fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragment_code.c_str());
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

	unsigned int shaderProgram = createShader("res/shaders/default/vertex.glsl", "res/shaders/default/fragment.glsl");

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
