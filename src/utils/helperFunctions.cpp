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
}