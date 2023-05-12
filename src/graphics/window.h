#pragma once

#include <iostream>
#include <unordered_map>

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

            double m_mouseX, m_mouseY;
            double m_previousMouseX, m_previousMouseY;

        public:
            Window(unsigned int _width, unsigned int _height, const char* _title);
            ~Window();
            bool Init();

            bool IsOpen() const { return !glfwWindowShouldClose(m_windowPtr); };
            void StartOfFrame();
            void EndOfFrame();

            void Terminate();

            bool IsKeyPressed(int _key) { return glfwGetKey(m_windowPtr, _key) == GLFW_PRESS; };
            bool IsMousePressed(int _mouseButton) { return glfwGetMouseButton(m_windowPtr, _mouseButton) == GLFW_PRESS; };
            double GetMouseX() const { return m_mouseX; };
            double GetMouseY() const { return m_mouseY; };
            double GetPreviousMouseX() const { return m_previousMouseX; };
            double GetPreviousMouseY() const { return m_previousMouseY; };
    };
}