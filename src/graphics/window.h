#pragma once

#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
    class Window
    {
        private:
            GLFWwindow* m_windowPtr;
            unsigned int m_defaultWidth;
            unsigned int m_defaultHeight;
            std::string m_title;

        public:
            Window(unsigned int _width, unsigned int _height, const char* _title);
            ~Window();
            bool Init();

            bool IsOpen() const { return !glfwWindowShouldClose(m_windowPtr); };
            void EndOfFrame();

            void Terminate();
    };
}