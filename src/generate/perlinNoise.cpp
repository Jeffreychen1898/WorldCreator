#include "perlinNoise.h"

namespace Generate
{
    PerlinNoise2D::PerlinNoise2D(unsigned int _gridSize)
        : m_gridSize(_gridSize)
    {
    }
            
    float PerlinNoise2D::Sample(float _x, float _y)
    {
        std::array<unsigned int, 4> corner_index = GetCornerIndex(_x, _y);
        std::array<glm::vec2, 4> gradient_vectors = GetGradientVectors(corner_index);
        std::array<glm::vec2, 4> distance_vectors = GetDistanceVectors(corner_index, _x / m_gridSize, _y / m_gridSize);

        float dot_products[4];
        for(unsigned int i=0;i<4;++i)
            dot_products[i] = glm::dot(gradient_vectors.at(i), distance_vectors.at(i));
        
        float horizontal_distance = _x / m_gridSize - corner_index.at(0);
        float vertical_distance = _y / m_gridSize - corner_index.at(2);

        float top_dot = Interpolate(dot_products[0], dot_products[1], horizontal_distance);
        float bottom_dot = Interpolate(dot_products[2], dot_products[3], horizontal_distance);

        float perlin_value = Interpolate(top_dot, bottom_dot, vertical_distance);
        
        return (perlin_value / 2.f) + 0.5f;
    }

    float PerlinNoise2D::Interpolate(float _start, float _end, float _weight)
    {
        return (_end - _start) * (3.f - _weight * 2.f) * _weight * _weight + _start;
        //return (_end - _start) * ((_weight * (_weight * 6.f - 15.f) + 10.f) * _weight * _weight * _weight) + _start;
    }

    std::array<glm::vec2, 4> PerlinNoise2D::GetDistanceVectors(std::array<unsigned int, 4>& _cornerIndexes, float _x, float _y)
    {
        glm::vec2 top_left = glm::vec2(_x - _cornerIndexes.at(0), _y - _cornerIndexes.at(2));
        glm::vec2 top_right = glm::vec2(_x - _cornerIndexes.at(1), _y - _cornerIndexes.at(2));
        glm::vec2 bottom_left = glm::vec2(_x - _cornerIndexes.at(0), _y - _cornerIndexes.at(3));
        glm::vec2 bottom_right = glm::vec2(_x - _cornerIndexes.at(1), _y - _cornerIndexes.at(3));

        std::array<glm::vec2, 4> result = { top_left, top_right, bottom_left, bottom_right };

        return result;
    }

    std::array<glm::vec2, 4> PerlinNoise2D::GetGradientVectors(std::array<unsigned int, 4>& _cornerIndexes)
    {
        glm::vec2 top_left = GetGradientVector(_cornerIndexes.at(0), _cornerIndexes.at(2));
        glm::vec2 top_right = GetGradientVector(_cornerIndexes.at(1), _cornerIndexes.at(2));
        glm::vec2 bottom_left = GetGradientVector(_cornerIndexes.at(0), _cornerIndexes.at(3));
        glm::vec2 bottom_right = GetGradientVector(_cornerIndexes.at(1), _cornerIndexes.at(3));

        std::array<glm::vec2, 4> result = { top_left, top_right, bottom_left, bottom_right };

        return result;
    }

    std::array<unsigned int, 4> PerlinNoise2D::GetCornerIndex(float _x, float _y)
    {
        unsigned int left_index = (unsigned int)(_x / m_gridSize);
        unsigned int top_index = (unsigned int)(_y / m_gridSize);
        unsigned int right_index = left_index + 1;
        unsigned int bottom_index = top_index + 1;

        std::array<unsigned int, 4> result = { left_index, right_index, top_index, bottom_index };

        return result;
    }

    glm::vec2 PerlinNoise2D::GetGradientVector(unsigned int _x, unsigned int _y)
    {
        unsigned long x_long = (unsigned long) _x;
        unsigned long y_long = (unsigned long) _y;

        unsigned long lookup_key = (x_long << 32) | y_long;
        if(m_gradientVectors.find(lookup_key) != m_gradientVectors.end())
            return m_gradientVectors[lookup_key];
        
        float angle = Utils::GetRandom() * 2.f * PI;

        glm::vec2 new_gradient_vector = glm::vec2(glm::cos(angle), glm::sin(angle));

        m_gradientVectors.insert({ lookup_key, new_gradient_vector });

        return new_gradient_vector;
    }
}