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
            unsigned int m_defaultWidth, m_defaultHeight;\
            unsigned int m_width, m_height;
            std::string m_title;

            double m_mouseX, m_mouseY;
            double m_previousMouseX, m_previousMouseY;

            double m_deltaScrollPosition;

        public:
            Window(unsigned int _width, unsigned int _height, const char* _title);
            ~Window();
            bool Init();

            bool IsOpen() const { return !glfwWindowShouldClose(m_windowPtr); };
            void StartOfFrame();
            void EndOfFrame();

            void SetDeltaScrollPosition(double _deltaPosition) { m_deltaScrollPosition += _deltaPosition; };

            void Terminate();

            void SetWindowDimension(unsigned int _width, unsigned int _height) { glViewport(0, 0, _width, _height); };

            bool IsKeyPressed(int _key) { return glfwGetKey(m_windowPtr, _key) == GLFW_PRESS; };
            bool IsMousePressed(int _mouseButton) { return glfwGetMouseButton(m_windowPtr, _mouseButton) == GLFW_PRESS; };
            double GetMouseX() const { return m_mouseX; };
            double GetMouseY() const { return m_mouseY; };
            double GetPreviousMouseX() const { return m_previousMouseX; };
            double GetPreviousMouseY() const { return m_previousMouseY; };
            double GetDeltaScrollPosition() const { return m_deltaScrollPosition; };
            unsigned int GetWidth() const { return m_width; };
            unsigned int GetHeight() const { return m_height; };
            GLFWwindow* GetWindow() { return m_windowPtr; };
    };

    static void G_ScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset);
    static void G_WindowResizeCallback(GLFWwindow* _window, int _width, int _height);
}