#include "stdafx.h"
#include "CCamera.h"
#include "CLogger.h"

using namespace std;

const float PI_VALUE = 3.141592f;
const float MAX_SPEED = 2.0f;
const float ACCELERATION = 8.0f;

CCamera::CCamera():
    m_Position(0.0f, 0.0f, 0.0f),
    m_LookAt(0.0f, 0.0f, 1.0f),
    m_Up(0.0f, 1.0f, 0.0f),
    m_Velocity(0.0f, 0.0f, 0.0f),
    m_fSpeedFactor(1.0f)
{
    memset(&m_Controls, 0, sizeof(m_Controls));
    
    m_matView = glm::mat4(1.0f);
    m_matProj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
}

void CCamera::setRotation(float fYaw, float fPitch)
{
    glm::vec3 NewDir(sinf(fYaw)*cosf(fPitch), sinf(fPitch), cosf(fYaw)*cosf(fPitch));
    m_LookAt = m_Position + NewDir;
    updateMatrix();
}

void CCamera::rotate(float fYaw, float fPitch)
{
    fYaw = -fYaw;
    if(fYaw == 0.0f && fPitch == 0.0f) return;
    
    glm::vec3 Dir = glm::normalize(m_LookAt - m_Position);
    
    fYaw += atan2(Dir.x, Dir.z);
    fYaw = fmodf(fYaw, 2.0f * PI_VALUE);
    
    fPitch += asinf(Dir.y);
    fPitch = max(min(fPitch, PI_VALUE / 2.0f - 0.1f), -(PI_VALUE / 2.0f - 0.1f));
    
    setRotation(fYaw, fPitch);
}

void CCamera::update(float dt)
{
    float fForward = 0.0f, fRight = 0.0f;
    if(m_Controls[CTRL_FORWARD])
        fForward += 1.0f;
    if(m_Controls[CTRL_BACK])
        fForward -= 1.0f;
    if(m_Controls[CTRL_RIGHT])
        fRight += 1.0f;
    if(m_Controls[CTRL_LEFT])
        fRight -= 1.0f;
    
    if(fForward != 0.0f || fRight != 0.0f)
    {
        glm::vec3 ForwardDir = glm::normalize(m_LookAt - m_Position);
        glm::vec3 RightDir = glm::normalize(-glm::cross(m_Up, ForwardDir));
        glm::vec3 TargetVelocity = ForwardDir * fForward + RightDir * fRight;
        TargetVelocity = glm::normalize(TargetVelocity) * MAX_SPEED;
        if(TargetVelocity != m_Velocity)
        {
            glm::vec3 VelocityDiff = TargetVelocity - m_Velocity;
            glm::vec3 Accel = glm::normalize(VelocityDiff) * dt * ACCELERATION;
            if(glm::length(Accel) > glm::length(VelocityDiff))
                Accel = VelocityDiff;
            m_Velocity += Accel;
        }
    }
    else
    {
        float fSpeed = glm::length(m_Velocity);
        if(fSpeed > 0.0f)
            m_Velocity = glm::normalize(m_Velocity) * max(fSpeed - dt * ACCELERATION, 0.0f);
    }
    
    glm::vec3 Move = m_Velocity * m_fSpeedFactor * dt;
    m_Position += Move;
    m_LookAt += Move;
    
    updateMatrix();
}

void CCamera::updateMatrix()
{
    m_matView = glm::lookAt(m_Position, m_LookAt, glm::vec3(0.0f, 1.0f, 0.0f));
    //m_matView = glm::scale(m_matView, glm::vec3(1.0f, 1.0f, -1.0f));
    
    //CLogger::getInstance().info("pos %.2f %.2f %.2f lookat %.2f %.2f %.2f\n",
    //    m_Position.x, m_Position.y, m_Position.z, m_LookAt.x, m_LookAt.y, m_LookAt.z);
}
