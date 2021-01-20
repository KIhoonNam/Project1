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
    // Ű�� ������ ī�޶� �������� ���ƴ� �ӵ��� �����մϴ�. ȸ�� �ӵ��� ������ ������.
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

    // ���� �������� ��ȯ�Ͻʽÿ�.
    float radians = m_rotationY * 0.0174532925f;

    // ��ġ�� ������Ʈ �մϴ�.
    m_position.x += sinf(radians) * m_forwardSpeed;
    m_position.z += cosf(radians) * m_forwardSpeed;

}

void PositionClass::MoveBack(bool keydown)
{
    // Ű�� ������ ī�޶� ���������� ȸ���ϴ� �ӵ��� �����մϴ�. ȸ�� �ӵ��� ������ ������.
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

    // ���� �������� ��ȯ�Ͻʽÿ�.
    float radians = m_rotationY * 0.0174532925f;

    // ��ġ�� ������Ʈ �մϴ�.
    m_position.x -= sinf(radians) * m_backwardSpeed;
    m_position.z -= cosf(radians) * m_backwardSpeed;
}




void PositionClass::TurnLeft(float keydown)
{
    // Ű�� ������ ī�޶� �������� ���ƴ� �ӵ��� �����մϴ�. ȸ�� �ӵ��� ������ ������.
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

    // ȸ�� �ӵ��� ����Ͽ� ȸ���� ������Ʈ�մϴ�.
    m_rotationY -= m_leftTurnSpeed;
    if (m_rotationY < 0.0f)
    {
        m_rotationY += 360.0f;
    }
}



void PositionClass::TurnRight(float keydown)
{
    // Ű�� ������ ī�޶� ���������� ȸ���ϴ� �ӵ��� �����մϴ�. ȸ�� �ӵ��� ������ ������.
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

    // ȸ�� �ӵ��� ����Ͽ� ȸ���� ������Ʈ�մϴ�.
    m_rotationY += m_rightTurnSpeed;
    if (m_rotationY > 360.0f)
    {
        m_rotationY -= 360.0f;
    }
}
