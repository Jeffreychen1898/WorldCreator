#pragma once

#include <chrono>
#include <iostream>

namespace Utils
{
    class Performance
    {
        private:
            std::chrono::high_resolution_clock::time_point m_previousTime;

        public:
            Performance()
            {
                ResetTime();
            };

            void ResetTime() {
                m_previousTime = std::chrono::high_resolution_clock::now();
            }

            float GetTimeMs()
            {
                std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();

                return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - m_previousTime).count();
            };
            float GetTimeSec() { return GetTimeMs() / 1e3; };
            float GetFps() { return 1e3 / GetTimeMs(); };
            float Dt() { return GetTimeMs() / 1e3; };

            void PrintTime() { std::cout << Dt() << std::endl; };
    };
}