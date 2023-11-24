#include "noiseHandler.h"

#define PERLIN_POPUP_WIDTH 300
#define PERLIN_POPUP_HEIGHT 500

namespace UI {
namespace Resource {
    void PerlinNoise_Popup()
    {
        PerlinNoise_PopupActive = true;
    }

    void PerlinNoise_PopupRender()
    {
        if(PerlinNoise_PopupActive)
        {
            ImGui::SetNextWindowSize(ImVec2(PERLIN_POPUP_WIDTH, PERLIN_POPUP_HEIGHT));
            ImGui::Begin("Create Perlin Noise Texture", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            
            ImGui::Text("%s", PerlinNoise_ErrorMessage);
            ImGui::Text("Width:");
            ImGui::Text("Height:");
            ImGui::Text("Grid Width:");
            ImGui::Text("Grid Height:");
            ImGui::End();
        }
    }
}}

#undef PERLIN_POPUP_WIDTH
#undef PERLIN_POPUP_HEIGHT