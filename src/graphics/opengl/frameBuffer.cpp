#include "frameBuffer.h"

namespace Graphics
{
    unsigned int FrameBuffer::s_currentFrameBuffer = 0;
    FrameBuffer::FrameBuffer(unsigned int _channels)
        : m_channels(_channels), m_texture(_channels), m_width(0), m_height(0), m_frameBuffer(0), m_renderBuffer(0)
    {
    }

    void FrameBuffer::Create(unsigned int _width, unsigned int _height, unsigned char _textureSettings)
    {
        /* create the frame buffer */
        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        /* create the texture */
        m_texture.Create(m_channels, _width, _height, nullptr, _textureSettings);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.GetId(), 0);

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