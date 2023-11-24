#pragma once

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace UI{ namespace Resource
{
    static bool PerlinNoise_PopupActive = false;
    static char PerlinNoise_ErrorMessage[] = "Error sasdf";
    
    void PerlinNoise_Popup();
    void PerlinNoise_PopupRender();
}}