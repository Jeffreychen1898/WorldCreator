#pragma once

#include "texture.h"

#include <glad.h>

namespace Graphics
{
    class FrameBuffer
    {
        public:
            static unsigned int s_currentFrameBuffer;

        private:
            unsigned int m_channels;

            unsigned int m_width, m_height;

            unsigned int m_frameBuffer, m_renderBuffer;
            Graphics::Texture m_texture;
            
        public:
            FrameBuffer(unsigned int _channels);
            ~FrameBuffer();

            void Create(unsigned int _width, unsigned int _height, unsigned char _textureSettings=TEX_SETTING_DEFAULT);

            void Clear();

            void Bind();
            static void BindDefault();

            unsigned int GetId() const { return m_frameBuffer; };
            unsigned int GetFrameBufferTextureId() { return m_texture.GetId(); };
            Graphics::Texture* GetTexture() { return &m_texture; };
    };
}