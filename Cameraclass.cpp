#include "Stdafx.h"
#include "TimerClass.h"
#include "Cameraclass.h"


CameraClass::CameraClass()
{  

    

    m_time = new TimerClass;

    // 위쪽을 가리키는 벡터를 설정합니다.
   

}


CameraClass::CameraClass(const CameraClass& other)
{

}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;

   
}


void CameraClass::SetRotation(float x, float y, float z)
{
   
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;

  
}

void CameraClass::RoationMatrix(float rotationx,float rotationy,float rotationz)
{
    m_rotation.x = rotationx;
    m_rotation.y = rotationy;
    m_rotation.z = rotationz;

    
}

void CameraClass::SetView()
{


  
}



void CameraClass::MoveFront(float position,float rotation)
{
    XMVECTOR Direction;
    Direction = lookAtVector - positionVector;
    XMVector3Normalize(Direction);
    XMVECTOR set = position * Direction;

    XMStoreFloat3(&lookAt, set);
    XMStoreFloat3(&m_position, set);




    
}

void CameraClass::MoveRight(float position)
{
    //XMVECTOR UpNormal, ForwardNormal, Direction, Croos;
    //XMVector3Normalize(XMVector3Cross(UpNormal, upVector));
    //XMVector3Normalize(XMVector3Cross(ForwardNormal, (lookAtVector - positionVector)));
    //Croos = XMVector3Cross(ForwardNormal, UpNormal);
    //XMVector3Cross(Direction, Croos);
    //XMVector3Normalize(Direction);
    //lookAtVector += position * Direction;
    //positionVector += position * Direction;
}




XMFLOAT3 CameraClass::GetPosition()
{
    return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
    return m_rotation;
}


void CameraClass::Render()
{

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    lookAt.x = 0;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;

    // XMVECTOR 구조체에 로드한다.
    upVector = XMLoadFloat3(&up);

    // 3D월드에서 카메라의 위치를 ​​설정합니다.
    position = m_position;

    // XMVECTOR 구조체에 로드한다.
    positionVector = XMLoadFloat3(&position);

    // 기본적으로 카메라가 찾고있는 위치를 설정합니다.
   

    // XMVECTOR 구조체에 로드한다.
    lookAtVector = XMLoadFloat3(&lookAt);

    // yaw (Y 축), pitch (X 축) 및 roll (Z 축)의 회전값을 라디안 단위로 설정합니다.
    pitch = m_rotation.x * 0.0174532925f;
    yaw = m_rotation.y * 0.0174532925f;
    roll = m_rotation.z * 0.0174532925f;

    //  yaw, pitch, roll 값을 통해 회전 행렬을 만듭니다.
    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // lookAt 및 up 벡터를 회전 행렬로 변형하여 뷰가 원점에서 올바르게 회전되도록 합니다.
    lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
    upVector = XMVector3TransformCoord(upVector, rotationMatrix);

  

  

    // 마지막으로 세 개의 업데이트 된 벡터에서 뷰 행렬을 만듭니다.
    m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
    viewMatrix = m_viewMatrix;
}
