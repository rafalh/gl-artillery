#ifndef CFRAMEBUFFER_H
#define CFRAMEBUFFER_H


class CFrameBuffer
{
    public:
        CFrameBuffer(int w, int h, bool bDepthBuffer = true);
        ~CFrameBuffer();
        
        GLuint getColor() const
        {
            return m_ColorTexture;
        }
        
        GLuint getDepth() const
        {
            return m_DepthTexture;
        }
        
        void bind();
        void unbind();
    
    private:
        int m_Width, m_Height;
        GLuint m_FBO, m_ColorTexture, m_DepthTexture;
        static GLint m_OldViewport[4];
};

#endif // CFRAMEBUFFER_H
