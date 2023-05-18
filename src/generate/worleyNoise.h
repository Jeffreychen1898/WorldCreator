#pragma once

#include <iostream>
#include <array>
#include <unordered_map>

#include "../utils/helperFunctions.h"

namespace Generate
{
    class WorleyNoise
    {
        private:
            int m_gridSize;
            float m_scale;
            /* lookup key (x and y combined) -> random points location */
            std::unordered_map<unsigned long, std::pair<float, float>> m_points;

        public:
            WorleyNoise(unsigned int _gridSize);

            float Sample(float _x, float _y);

        private:
            void GetPoint(float& _outputX, float& _outputY, int _x, int _y);
    };
}