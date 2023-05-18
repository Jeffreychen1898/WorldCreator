#pragma once

#include <random>

#define PI 3.1415926535

namespace Utils
{
    static std::uniform_real_distribution<float>* g_randomDistribution;
    static std::mt19937* g_randomGenerator;
    
    void HelperInit();
    void HelperClose();

    float GetRandom();
    int Floor(float _value);
}