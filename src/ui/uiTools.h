#pragma once

#include <cmath>
#include <iostream>
#include <list>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../utils/helperFunctions.h"
#include "noiseHandler.h"

namespace UI
{
    class UITools
    {
        private:
            float m_gammaCorrection;
            float m_exposure;

            float m_sunDistance;
            float m_sunAnglePhi;
            float m_sunAngleTheta;
            float m_sunColor[3];
            float m_sunIntensity;

        public:
            UITools();
            ~UITools();

            void DisplayUI(float _fps);

            float GetSunPositionX() const { return std::sin(Utils::ToRad(m_sunAngleTheta)) * std::cos(Utils::ToRad(m_sunAnglePhi)) * m_sunDistance; };
            float GetSunPositionY() const { return std::cos(Utils::ToRad(m_sunAngleTheta)) * m_sunDistance; };
            float GetSunPositionZ() const { return std::cos(Utils::ToRad(m_sunAngleTheta)) * std::sin(Utils::ToRad(m_sunAnglePhi)) * m_sunDistance; };
            float GetSunColorRed() const { return m_sunColor[0]; };
            float GetSunColorGreen() const { return m_sunColor[1]; };
            float GetSunColorBlue() const { return m_sunColor[2]; };
            float GetSunIntensity() const { return m_sunIntensity; };
            float GetGammaCorrection() const { return m_gammaCorrection; };
            float GetExposure() const { return m_exposure; };

        private:
            void TriggerNewPerlinNoisePopup();
    };
}