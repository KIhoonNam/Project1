#pragma once



class PositionClass
{
public:
    PositionClass();
    PositionClass(const PositionClass&);
    ~PositionClass();

    void SetFrameTime(float);
    void GetRotation(float&);
    float GetPosX();
    float GetPosZ();


    void MoveLeft(bool);
    void MoveRight(bool);
    void MoveFront(bool);
    void MoveBack(bool);
    void TurnLeft(float);
    void TurnRight(float);

private:
    float m_frameTime = 0.0f;;
    float m_rotationY = 0.0f;;
    float m_leftTurnSpeed = 0.0f;
    float m_rightTurnSpeed = 0.0f;;

    float m_forwardSpeed;
    float m_backwardSpeed;
    XMFLOAT3 m_position = XMFLOAT3(0, 0, 0);
};
