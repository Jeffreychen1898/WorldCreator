#pragma once

#include <iostream>

#include <glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace UI
{
    class UserInterface
    {
        private:
            unsigned int m_windowWidth;
            unsigned int m_windowHeight;

            unsigned int m_toolsWidth;
            unsigned int m_propertiesWidth;
            unsigned int m_resHeight;

            bool m_isPropertiesWidthSet;
            bool m_isToolsWidthSet;
            bool m_isResHeightSet;

            ImGuiWindowFlags m_mainWindowFlags;

            unsigned int m_mainFramebuffer;
            float m_mainFrameBufferAspectRatio;

            std::string m_windowFocused;
        public:
            UserInterface();
            ~UserInterface();

            void Init(GLFWwindow* _glfwWindow, const char* _glslVersion);
            void StartOfFrame(unsigned int _windowWidth, unsigned int _windowHeight);
            void EndOfFrame();
            void DisplayUI();

            void SetMainFrameBuffer(unsigned int _frameBuffer) { m_mainFramebuffer = _frameBuffer; };

            std::string GetFocusedWindow() const { return m_windowFocused; };
        
        private:
            void CreateWindow(const char* _windowName, ImGuiWindowFlags _windowFlags);
            void CreateWindow(const char* _windowName);
            void DisplayToolsWindow();
            void DisplayRenderWindow();
            void DisplayResWindow();
            void DisplayPropertiesWindow();
    };
}