#pragma once

#include <iostream>
#include <list>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define UI_ELEM_TEXT 0
#define UI_ELEM_INPUT 1
#define UI_ELEM_BUTTON 2
#define UI_ELEM_SLIDER 3

namespace UI
{
    struct PopupElement
    {
        unsigned int type;
        std::string content;
        int rangeMin;
        int rangeMax;
        int defaultValue;
        std::string input = "";
    };

    struct PopupInfo
    {
        unsigned int width;
        unsigned int height;
        std::string windowName;
        std::list<UI::PopupElement>* elements;
    };

    class PopupHandler
    {
        private:
            typedef void (PopupHandler::*PopupHandlerFuncPtr)();

            std::list<PopupInfo>* m_popupInfo;
            std::list<PopupHandlerFuncPtr>* m_popupSubmitFunc;
        public:
            PopupHandler();
            ~PopupHandler();

            void InitPerlinNoisePopup();

            void RenderPopups();
        
        private:
            void HandlePerlinNoise();
    };
}