#include "popups.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace UI
{
    PopupHandler::PopupHandler()
    {
        m_popupInfo = new std::list<PopupInfo>();
        m_popupSubmitFunc = new std::list<PopupHandlerFuncPtr>();
    }

    void PopupHandler::InitPerlinNoisePopup()
    {
        //PopupHandlerFuncPtr myfunc = &PopupHandler::HandlePerlinNoise;
        //(this->*myfunc)();
        m_popupSubmitFunc->push_back(&PopupHandler::HandlePerlinNoise);

        std::list<UI::PopupElement>* window_elements = new std::list<UI::PopupElement>();
        window_elements->push_back({ UI_ELEM_INPUT, "Grid Width:", 0, 5, 50});
        window_elements->push_back({ UI_ELEM_INPUT, "Grid Height:", 0, 5, 50});

        PopupInfo window_information = { 300, 500, "Create Perlin Noise Texture", window_elements };

        m_popupInfo->push_back(window_information);
    }

    void PopupHandler::RenderPopups()
    {
        for(PopupInfo& popup_info : *m_popupInfo)
        {
            ImGui::SetNextWindowSize(ImVec2(popup_info.width, popup_info.height));
            ImGui::Begin(popup_info.windowName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            // render the window elements
            for(PopupElement& each_element : *popup_info.elements)
            {
                if(each_element.type == UI_ELEM_TEXT)
                    ImGui::Text(each_element.content.c_str());
                else if(each_element.type == UI_ELEM_INPUT)
                {
                    char* inputBuffer = new char[each_element.rangeMax + 1];
                    strcpy(inputBuffer, each_element.input.c_str());
                    ImGui::InputText(each_element.content.c_str(), inputBuffer, each_element.rangeMax);
                    each_element.input = inputBuffer;
                    delete inputBuffer;
                }
            }

            ImGui::End();
        }
    }

    void PopupHandler::HandlePerlinNoise()
    {
        std::cout << "called" << std::endl;
    }

    PopupHandler::~PopupHandler()
    {
        // delete m_popupInfo;
        // delete m_popupSubmitFunc
    }
}