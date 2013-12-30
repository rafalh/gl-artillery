#ifndef CCAMERA_H
#define CCAMERA_H

#include "CVector.h"

class CCamera
{
    public:
        enum EControl
        {
            CTRL_FORWARD, CTRL_BACK, CTRL_RIGHT, CTRL_LEFT, CTRL_COUNT
        };
        
        CCamera();
        void setRotation(float fYaw, float fPitch);
        void rotate(float fYaw, float fPitch);
        void update(float dt);
        
        void setControlState(EControl Ctrl, bool bActive)
        {
            m_Controls[Ctrl] = bActive;
        }
        
        const glm::vec3 &getPosition() const
        {
            return m_Position;
        }
        
        void setPosition(const glm::vec3 &Pos)
        {
            m_Position = Pos;
        }
        
        const glm::vec3 &getLookAt() const
        {
            return m_LookAt;
        }
        
        void setLookAt(const glm::vec3 &LookAt)
        {
            m_LookAt = LookAt;
        }
        
        void setSpeed(float fSpeed)
        {
            m_fSpeedFactor = fSpeed;
        }
        
        const glm::mat4 &getViewMatrix()
        {
            return m_matView;
        }
        
        const glm::mat4 &getProjMatrix()
        {
            return m_matProj;
        }
        
    private:
        glm::vec3 m_Position;
        glm::vec3 m_LookAt;
        glm::vec3 m_Up;
        glm::vec3 m_Velocity;
        bool m_Controls[CTRL_COUNT];
        float m_fSpeedFactor;
        glm::mat4 m_matView, m_matProj;
        
        void updateMatrix();
};

#endif // CCAMERA_H
