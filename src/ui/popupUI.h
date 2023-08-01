#pragma once

#include <iostream>

namespace UI
{
    class PopupUI
    {
        private:
        public:
            PopupUI();
            ~PopupUI();

            void InvokePopup() const { std::cout << "popup invoked!" << std::endl; };
    };
}