#include "window.h"

namespace Graphics
{
    Window::Window(unsigned int _width, unsigned int _height, const char* _title)
        : m_defaultWidth(_width), m_defaultHeight(_height), m_title(_title), m_windowPtr(nullptr),
        m_mouseX(0), m_mouseY(0), m_previousMouseX(0), m_previousMouseY(0)
    {
    }

    Window::~Window()
    {
        Terminate();
    }

    bool Window::Init()
    {
        /* initialize glfw */
        if(!glfwInit())
		    return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* create glfw window */
        m_windowPtr = glfwCreateWindow(m_defaultWidth, m_defaultHeight, m_title.c_str(), NULL, NULL);
        m_width = m_defaultWidth;
        m_height = m_defaultHeight;
	    if(!m_windowPtr)
	    {
	    	Terminate();
	    	return false;
	    }

        /* make context current and load glad */
	    glfwMakeContextCurrent(m_windowPtr);

	    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	    {
	    	std::cout << "[Error] Failed to initialize GLAD" << std::endl;
	    	return false;
	    }

        /* reference this class */
        glfwSetWindowUserPointer(m_windowPtr, this);

        /* set window events */
        glfwSetWindowSizeCallback(m_windowPtr, G_WindowResizeCallback);
        glfwSetScrollCallback(m_windowPtr, G_ScrollCallback);

        return true;
    }

    void Window::StartOfFrame()
    {
        m_previousMouseX = m_mouseX;
        m_previousMouseY = m_mouseY;
        glfwGetCursorPos(m_windowPtr, &m_mouseX, &m_mouseY);

        int window_width, window_height;
        glfwGetWindowSize(m_windowPtr, &window_width, &window_height);
        m_width = window_width;
        m_height = window_height;
    }

    void Window::EndOfFrame()
    {
        m_deltaScrollPosition = 0;
        
        glfwSwapBuffers(m_windowPtr);
		glfwPollEvents();
    }

    void Window::Terminate()
    {
        glfwTerminate();
    }

    void G_ScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset)
    {
        Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(_window));
        instance->SetDeltaScrollPosition(_yoffset);
    }

    void G_WindowResizeCallback(GLFWwindow* _window, int _width, int _height)
    {
        /* not necessary anymore since we are rendering to a buffer of constant size */
        /*Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(_window));
        instance->SetWindowDimension(_width, _height);*/
    }
}