#pragma once

#include <iostream>
#include <unordered_map>

#include <glad.h>
#include <GLFW/glfw3.h>

#define TEX_S_CLAMP_EDGE     0x80      // 1000 0000
#define TEX_S_REPEAT         0x00      // 0000 0000
#define TEX_T_CLAMP_EDGE     0x40      // 0100 0000
#define TEX_T_REPEAT         0x00      // 0000 0000
#define TEX_MIN_NEAREST      0x20      // 0010 0000
#define TEX_MIN_LINEAR       0x00      // 0000 0000
#define TEX_MAG_NEAREST      0x10      // 0001 0000
#define TEX_MAG_LINEAR       0x00      // 0000 0000

#define TEX_SETTING_DEFAULT 0xC0       // 1100 0000

namespace Graphics {
    class Texture {
        private:
            unsigned int m_textureId;
            unsigned int m_textureChannels;

            /* texture slot -> texture id */
            static std::unordered_map<unsigned int, unsigned int> s_texSlotBindings;

        public:
            Texture(unsigned int _textureChannels);
            ~Texture();

            void Create(unsigned int _inputChannels, unsigned int _width, unsigned int _height, unsigned char* _data, unsigned char _settings=TEX_SETTING_DEFAULT);
            void Create(const char* _path, unsigned char _settings=TEX_SETTING_DEFAULT) {}; // not implemented just yet

            void Bind(unsigned int _texSlot);
    };
}