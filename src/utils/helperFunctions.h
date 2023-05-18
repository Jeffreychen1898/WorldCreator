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
    float RandRange(float _lower, float _upper);

    int Floor(float _value);

    float Qrsqrt(float _number); /* using the famous quick inverse sqrt function :D */
    float Qsqrt(float _number);
    float Distance(float _x1, float _y1, float _x2, float _y2);
}