#include "texture.h"

namespace Graphics {
    std::unordered_map<unsigned int, unsigned int> Graphics::Texture::s_texSlotBindings;
    Texture::Texture(unsigned int _textureChannels)
        : m_textureId(0), m_textureChannels(_textureChannels)
    {
    }

    void Texture::Create(unsigned int _inputChannels, unsigned int _width, unsigned int _height, unsigned char* _data, unsigned char _settings)
    {
        glGenTextures(1, &m_textureId);
        Bind(0);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        GLenum min_filter = (_settings & TEX_MIN_NEAREST) > 0 ? GL_NEAREST : GL_LINEAR;
        GLenum mag_filter = (_settings & TEX_MAG_NEAREST) > 0 ? GL_NEAREST : GL_LINEAR;
        GLenum wrap_s_coord = (_settings & TEX_S_CLAMP_EDGE) > 0 ? GL_CLAMP_TO_EDGE : GL_REPEAT;
        GLenum wrap_t_coord = (_settings & TEX_T_CLAMP_EDGE) > 0 ? GL_CLAMP_TO_EDGE : GL_REPEAT;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_coord);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_coord);

        GLenum internal_format = GL_RGBA8;
        switch(m_textureChannels) {
            case TEX_R:
                internal_format = GL_R8;
                break;
            case TEX_RG:
                internal_format = GL_RG8;
                break;
            case TEX_RGB:
                internal_format = GL_RGB8;
                break;
            case TEX_RGBA:
                internal_format = GL_RGBA8;
                break;
            case TEX_R_UI:
                internal_format = GL_R32I;
                break;
            case TEX_RG_UI:
                internal_format = GL_RG32I;
                break;
            case TEX_RGB_UI:
                internal_format = GL_RGB32I;
                break;
            case TEX_RGBA_UI:
                internal_format = GL_RGBA32I;
                break;
        }

        GLenum data_format = GL_RED;
        switch(_inputChannels) {
            case TEX_R:
                data_format = GL_RED;
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                break;
            case TEX_RG:
                data_format = GL_RG;
                break;
            case TEX_RGB:
                data_format = GL_RGB;
                break;
            case TEX_RGBA:
                data_format = GL_RGBA;
                break;
            case TEX_R_UI:
                data_format = GL_RED_INTEGER;
                break;
            case TEX_RG_UI:
                data_format = GL_RG_INTEGER;
                break;
            case TEX_RGB_UI:
                data_format = GL_RGB_INTEGER;
                break;
            case TEX_RGBA_UI:
                data_format = GL_RGBA_INTEGER;
                break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, _width, _height, 0, data_format, GL_UNSIGNED_BYTE, _data);
        if(data_format == GL_RED)
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        Bind(0);
    }

    void Texture::Bind(unsigned int _texSlot)
    {
        if(s_texSlotBindings.find(_texSlot) == s_texSlotBindings.end())
        {
            glActiveTexture(GL_TEXTURE0 + _texSlot);
            glBindTexture(GL_TEXTURE_2D, m_textureId);

            s_texSlotBindings.insert({ _texSlot, m_textureId });
            return;
        }

        if(s_texSlotBindings[_texSlot] == m_textureId)
            return;
        
        glActiveTexture(GL_TEXTURE0 + _texSlot);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        s_texSlotBindings[_texSlot] = m_textureId;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_textureId);
    }
}