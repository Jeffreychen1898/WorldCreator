#include "helperFunctions.h"

namespace Utils
{
    void HelperInit()
    {
        g_randomGenerator = new std::mt19937(std::random_device{}());
        g_randomDistribution = new std::uniform_real_distribution<float>(0.f, 1.f);
    }

    void HelperClose()
    {
        if(g_randomDistribution != nullptr) delete g_randomDistribution;
        if(g_randomGenerator != nullptr) delete g_randomGenerator;
    }

    float GetRandom()
    {
        return (*g_randomDistribution)(*g_randomGenerator);
    }

    float RandRange(float _lower, float _upper)
    {
        return Utils::GetRandom() * (_upper - _lower) + _lower;
    }

    int Floor(float _value)
    {
        int converted_value = (int)_value;
        if(converted_value == _value) return converted_value;
        return _value < 0 ? converted_value - 1 : converted_value;
    }

    float Qrsqrt(float _number)
    {
        long i;
        float x2, y;
        const float three_halfs = 1.5f;

        x2 = _number * 0.5f;
        y = _number;
        i = *(long*)&y;
        i = 0x5f3759df - (i >> 1);
        y = * (float*) &i;
        y = y * (three_halfs - (x2 * y * y));

        return y;
    }

    float Qsqrt(float _number)
    {
        return 1.0 / Qrsqrt(_number);
    }

    float Distance(float _x1, float _y1, float _x2, float _y2)
    {
        return Utils::Qsqrt((_x2 - _x1) * (_x2 - _x1) + (_y2 - _y1) * (_y2 - _y1));
    }

    float ToDeg(float _ang)
    {
        return _ang * 180.f / PI;
    }

    float ToRad(float _ang)
    {
        return _ang * PI / 180.f;
    }
}