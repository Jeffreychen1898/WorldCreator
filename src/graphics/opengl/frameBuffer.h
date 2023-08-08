#pragma once

#include <vector>

#include "texture.h"

#include <glad.h>

namespace Graphics
{
    class FrameBuffer
    {
        public:
            static unsigned int s_currentFrameBuffer;

        private:
            unsigned int m_width, m_height;

            unsigned int m_attachments;

            unsigned int m_frameBuffer, m_renderBuffer;
            std::vector<Graphics::Texture*> m_textures;
            
        public:
            FrameBuffer(unsigned int _attachments);
            ~FrameBuffer();

            void Create(unsigned int _width, unsigned int _height, unsigned int _channels[], unsigned char _textureSettings=TEX_SETTING_DEFAULT);

            void Clear();

            void Bind();
            static void BindDefault();

            unsigned int GetId() const { return m_frameBuffer; };
            unsigned int GetFrameBufferTextureId(unsigned int _attachment) { return m_textures.at(_attachment)->GetId(); };
            Graphics::Texture* GetTexture(unsigned int _attachment) { return m_textures.at(_attachment); };

            int ReadPixel(unsigned int _attachment, unsigned int _x, unsigned int _y);
    };
}