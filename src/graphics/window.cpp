#include "window.h"

namespace Graphics
{
    Window::Window(unsigned int _width, unsigned int _height, const char* _title)
        : m_defaultWidth(_width), m_defaultHeight(_height), m_title(_title), m_windowPtr(nullptr)
    {
    }

    Window::~Window()
    {
        Terminate();
    }

    bool Window::Init()
    {
        if(!glfwInit())
		    return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_windowPtr = glfwCreateWindow(m_defaultWidth, m_defaultHeight, m_title.c_str(), NULL, NULL);
	    if(!m_windowPtr)
	    {
	    	Terminate();
	    	return false;
	    }

	    glfwMakeContextCurrent(m_windowPtr);

	    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	    {
	    	std::cout << "[Error] Failed to initialize GLAD" << std::endl;
	    	return false;
	    }

        return true;
    }

    void Window::EndOfFrame()
    {
        glfwSwapBuffers(m_windowPtr);
		glfwPollEvents();
    }

    void Window::Terminate()
    {
        glfwTerminate();
    }

}