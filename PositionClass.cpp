#include "stdafx.h"
#include "PositionClass.h"


PositionClass::PositionClass()
{
   
}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}


void PositionClass::SetFrameTime(float time)
{
    m_frameTime = time;
}


void PositionClass::GetRotation(float& y)
{
    y = m_rotationY;
}

float PositionClass::GetPosX()
{
    return m_position.x;
}

float PositionClass::GetPosZ()
{
    return m_position.z;
}

void PositionClass::MoveLeft(bool keydown)
{
   
}

void PositionClass::MoveRight(bool keydown)
{

}

void PositionClass::MoveFront(bool keydown)
{
    // 키를 누르면 카메라가 왼쪽으로 돌아는 속도가 증가합니다. 회전 속도를 늦추지 않으면.
    if (keydown)
    {
        m_forwardSpeed += m_frameTime * 0.001f;

        if (m_forwardSpeed > (m_frameTime * 0.03f))
        {
            m_forwardSpeed = m_frameTime * 0.03f;
        }
    }
    else
    {
        m_forwardSpeed -= m_frameTime * 0.0007f;

        if (m_forwardSpeed < 0.0f)
        {
            m_forwardSpeed = 0.0f;
        }
    }

    // 값을 라디안으로 변환하십시오.
    float radians = m_rotationY * 0.0174532925f;

    // 위치를 업데이트 합니다.
    m_position.x += sinf(radians) * m_forwardSpeed;
    m_position.z += cosf(radians) * m_forwardSpeed;

}

void PositionClass::MoveBack(bool keydown)
{
    // 키를 누르면 카메라가 오른쪽으로 회전하는 속도가 증가합니다. 회전 속도를 늦추지 않으면.
    if (keydown)
    {
        m_backwardSpeed += m_frameTime * 0.001f;

        if (m_backwardSpeed > (m_frameTime * 0.03f))
        {
            m_backwardSpeed = m_frameTime * 0.03f;
        }
    }
    else
    {
        m_backwardSpeed -= m_frameTime * 0.0007f;

        if (m_backwardSpeed < 0.0f)
        {
            m_backwardSpeed = 0.0f;
        }
    }

    // 값을 라디안으로 변환하십시오.
    float radians = m_rotationY * 0.0174532925f;

    // 위치를 업데이트 합니다.
    m_position.x -= sinf(radians) * m_backwardSpeed;
    m_position.z -= cosf(radians) * m_backwardSpeed;
}




void PositionClass::TurnLeft(float keydown)
{
    // 키를 누르면 카메라가 왼쪽으로 돌아는 속도가 증가합니다. 회전 속도를 늦추지 않으면.
    if (keydown == -1)
    {
        m_leftTurnSpeed += m_frameTime * 0.1f;

        if (m_leftTurnSpeed > (m_frameTime * 1.0f))
        {
            m_leftTurnSpeed = m_frameTime * 1.0f;
        }
    }
    else
    {
        m_leftTurnSpeed -= m_frameTime * 0.005f;

        if (m_leftTurnSpeed < 0.0f)
        {
            m_leftTurnSpeed = 0.0f;
        }
    }

    // 회전 속도를 사용하여 회전을 업데이트합니다.
    m_rotationY -= m_leftTurnSpeed;
    if (m_rotationY < 0.0f)
    {
        m_rotationY += 360.0f;
    }
}



void PositionClass::TurnRight(float keydown)
{
    // 키를 누르면 카메라가 오른쪽으로 회전하는 속도가 증가합니다. 회전 속도를 늦추지 않으면.
    if (keydown == -1)
    {
        m_rightTurnSpeed += m_frameTime * 0.1f;

        if (m_rightTurnSpeed > (m_frameTime * 1.0f))
        {
            m_rightTurnSpeed = m_frameTime * 1.0f;
        }
    }
    else
    {
        m_rightTurnSpeed -= m_frameTime * 0.005f;

        if (m_rightTurnSpeed < 0.0f)
        {
            m_rightTurnSpeed = 0.0f;
        }
    }

    // 회전 속도를 사용하여 회전을 업데이트합니다.
    m_rotationY += m_rightTurnSpeed;
    if (m_rotationY > 360.0f)
    {
        m_rotationY -= 360.0f;
    }
}
