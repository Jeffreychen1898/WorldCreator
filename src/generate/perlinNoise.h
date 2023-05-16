#pragma once

#include <iostream>
#include <unordered_map>
#include <array>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../utils/helperFunctions.h"

namespace Generate
{
    class PerlinNoise2D
    {
        private:
            unsigned int m_gridSize;

            /* lookup key (x and y combined) -> gradient vector */
            std::unordered_map<unsigned long, glm::vec2> m_gradientVectors;
        public:
            PerlinNoise2D(unsigned int _gridSize);
            
            float Sample(float _x, float _y);

        private:
            float Interpolate(float _start, float _end, float _weight);
            std::array<glm::vec2, 4> GetDistanceVectors(std::array<unsigned int, 4>& _cornerIndexes, float _x, float _y);
            std::array<glm::vec2, 4> GetGradientVectors(std::array<unsigned int, 4>& _cornerIndexes);
            std::array<unsigned int, 4> GetCornerIndex(float _x, float _y);
            glm::vec2 GetGradientVector(unsigned int _x, unsigned int _y);
    };
}