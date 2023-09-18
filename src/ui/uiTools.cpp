#include "uiTools.h"

namespace UI
{
    UITools::UITools()
        : m_gammaCorrection(2.2), m_exposure(1), m_sunDistance(1000), m_sunAnglePhi(0), m_sunAngleTheta(0.f), m_sunIntensity(1), m_popupHandler(nullptr)
    {
        for(unsigned int i=0;i<3;++i)
            m_sunColor[i] = 1.f;
    }

    void UITools::Init(UI::PopupHandler* _popupHandler)
    {
        m_popupHandler = _popupHandler;
    }

    void UITools::DisplayUI(float _fps)
    {
        ImGuiStyle& imgui_style = ImGui::GetStyle();

        std::string frame_rate_string = "Frame Rate: " + std::to_string(_fps);
        ImGui::Text(frame_rate_string.c_str());

        if(ImGui::CollapsingHeader("Rendering"))
        {
            ImGui::Text("Gamma Correction");
            ImGui::SliderFloat("Gamma Correction", &m_gammaCorrection, 0.f, 10.f, "%.2f");

            ImGui::Text("Exposure");
            ImGui::SliderFloat("Exposure", &m_exposure, 0.f, 10.f, "%.2f");

            ImGui::Text("Sun Angle Phi");
            ImGui::DragFloat("Sun Angle Phi", &m_sunAnglePhi, 1.f, 0.f, 360.f, "%.2f", ImGuiSliderFlags_Logarithmic);

            ImGui::Text("Sun Angle Theta");
            ImGui::DragFloat("Sun Angle Theta", &m_sunAngleTheta, 0.5f, 0.f, 180.f, "%.2f", ImGuiSliderFlags_Logarithmic);

            ImGui::Text("Sun Distance");
            ImGui::DragFloat("Sun Distance", &m_sunDistance, 1000.f, 0, 10e6, "%.1f", ImGuiSliderFlags_Logarithmic);

            ImGui::Text("Sun Intensity");
            ImGui::DragFloat("Sun Intensity", &m_sunIntensity, 0.01f, 0, 10.f, "%.3f", ImGuiSliderFlags_Logarithmic);

            ImGui::Text("Sun Color");
            ImGui::ColorEdit3("Sun Color", (float*)&m_sunColor);
        }

        if(ImGui::CollapsingHeader("Controls"))
        {
            ImGui::Text("Controls coming soon:");
            ImGui::Text("\tMovement Speed");
            ImGui::Text("\tSpeed Curve");
            ImGui::Text("\tZoom Speed");
            ImGui::Text("\tZoom Speed Curve");
        }

        if(ImGui::CollapsingHeader("New Object"))
        {
            ImGui::Text("Shapes");
            ImGui::Button("Plane");
            ImGui::Text("Value 2D");
            if(ImGui::Button("Perlin Noise")) {
                // trigger popup
                //std::cout << "Trigger popup" << std::endl;
                m_popupHandler->InitPerlinNoisePopup();
            }
            ImGui::Text("Functions");
            ImGui::Button("Functions");
        }
    }

    void UITools::TriggerNewPerlinNoisePopup() {
        //ImGui::Begin();
    }

    UITools::~UITools()
    {
        //
    }
}