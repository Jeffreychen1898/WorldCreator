#include "frameBuffer.h"

namespace Graphics
{
    unsigned int FrameBuffer::s_currentFrameBuffer = 0;
    FrameBuffer::FrameBuffer(unsigned int _attachments)
        : m_width(0), m_height(0), m_frameBuffer(0), m_renderBuffer(0), m_attachments(_attachments)
    {
    }

    void FrameBuffer::Create(unsigned int _width, unsigned int _height, unsigned int _channels[], unsigned char _textureSettings)
    {
        /* create the frame buffer */
        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        /* create the texture */
        GLenum attachments_[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        for(unsigned int i=0;i<m_attachments;++i)
        {
            Graphics::Texture* each_texture = new Graphics::Texture(_channels[i]);
            each_texture->Create(_channels[i], _width, _height, nullptr, _textureSettings);
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachments_[i], GL_TEXTURE_2D, each_texture->GetId(), 0);
            std::cout << _width << " : " << _height << " : " << _channels[i] << std::endl;
            m_textures.push_back(each_texture);
        }
        /*for(unsigned int i=0;i<m_textures.size();++i)
            std::cout << m_textures[i]->GetId() << std::endl;*/

        /* create the render buffer */
        glGenRenderbuffers(1, &m_renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

        #if DEBUG_MODE
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "[Error] Failed to create frame buffer!" << std::endl;
        #endif

        m_width = _width;
        m_height = _height;

        s_currentFrameBuffer = m_frameBuffer;
    }

    int FrameBuffer::ReadPixel(unsigned int _attachment, unsigned int _x, unsigned int _y)
    {
        m_textures[_attachment]->Bind(0);
        int data[m_width * m_height];
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_INT, data);

        return data[_y * m_width + _x];
    }

    void FrameBuffer::Clear()
    {
        Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void FrameBuffer::Bind()
    {
        if(s_currentFrameBuffer == m_frameBuffer)
            return;
        
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        s_currentFrameBuffer = m_frameBuffer;
    }

    void FrameBuffer::BindDefault()
    {
        if(s_currentFrameBuffer == 0)
            return;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        s_currentFrameBuffer = 0;
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &m_frameBuffer);
        glDeleteRenderbuffers(1, &m_renderBuffer);
        s_currentFrameBuffer = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}