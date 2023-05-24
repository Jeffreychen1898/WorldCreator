#include "worleyNoise.h"

namespace Generate
{
    WorleyNoise::WorleyNoise(unsigned int _gridSize)
        : m_gridSize(_gridSize)
    {
        /* maximum distance is */
        /* current sample point is at the corner of its cell */
        /* all adjacent cells have a point at the opposite corner */
        m_scale = Utils::Qrsqrt(2 * _gridSize * _gridSize);
    }

    float WorleyNoise::Sample(float _x, float _y)
    {
        int index_x = Utils::Floor(_x / m_gridSize);
        int index_y = Utils::Floor(_y / m_gridSize);

        float shortest_distance = -1;
        for(int i=0;i<3;++i)
        {
            for(int j=0;j<3;++j)
            {
                float grid_x, grid_y;
                GetPoint(grid_x, grid_y, index_x - 1 + j, index_y - 1 + i);
                float distance = Utils::Distance(grid_x, grid_y, _x, _y);

                shortest_distance = (shortest_distance == -1 || shortest_distance > distance) ? distance : shortest_distance;
            }
        }

        return shortest_distance * m_scale;
    }

    void WorleyNoise::GetPoint(float& _outputX, float& _outputY, int _x, int _y)
    {
        unsigned long x_long = *((unsigned long*)&_x);
        unsigned long y_long = *((unsigned long*)&_y);

        unsigned long lookup_key = (x_long << 32) | y_long;

        if(m_points.find(lookup_key) != m_points.end())
        {
            std::pair<float, float> point_position = m_points[lookup_key];
            _outputX = point_position.first;
            _outputY = point_position.second;
            return;
        }
        
        float generate_point_x = Utils::RandRange(_x * m_gridSize, (_x + 1) * m_gridSize);
        float generate_point_y = Utils::RandRange(_y * m_gridSize, (_y + 1) * m_gridSize);

        m_points.insert({ lookup_key, { generate_point_x, generate_point_y }});

        _outputX = generate_point_x;
        _outputY = generate_point_y;
    }
}