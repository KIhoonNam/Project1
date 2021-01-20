#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "lightshaderclass.h"
#include "Colorshaderclass.h"
#include "boxclass.h"
#include "Grid.h"
#include "Bitmap.h"
#include "TextureShaderClass.h"
#include "Textclass.h"
#include "modellistclass.h"
#include "Multitextureshaderclass.h"
#include "rendertextureclass.h"
#include "debugwindowclass.h"
#include "frustumclass.h"
#include "graphicsclass.h"



GraphicsClass::GraphicsClass()
{
    m_rotationX = 60;
    m_rotationY = 0;
    m_rotationZ = 0;
    m_positionX = 0;
    m_positionY = 40;
    m_positionZ = -40;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    // Direct3D 객체 생성
    m_Direct3D = new D3DClass;
    if (!m_Direct3D)
    {
        return false;
    }

    // Direct3D 객체 초기화
    if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
    {
        MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
        return false;
    }  // m_Camera 객체 생성
    m_Camera = new CameraClass;
    if (!m_Camera)
    {
        return false;
    }
    XMMATRIX baseViewMatrix;
    m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
    m_Camera->Render();
    m_Camera->GetViewMatrix(baseViewMatrix);
    // m_Text 객체 생성
    m_Text = new TextClass;
    if (!m_Text)
    {
        return false;
    }

    // m_Text 객체 초기화
    if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight,
        baseViewMatrix))
    {
        MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
        return false;
    }
  

    m_Camera->SetPosition(m_positionX, m_positionY, m_positionZ);
    m_Camera->SetRotation(m_rotationX, m_rotationY, m_rotationZ); 
    // m_Model 객체 생성
    m_Model = new ModelClass;
    if (!m_Model)
    {
        return false;
    }
    char a[] = "../Project1/data/cube.txt";
    WCHAR dds1[] = L"../Project1/data/stone01.dds";
    WCHAR dds2[] = L"../Project1/data/dirt01.dds";
    // m_Model 객체 초기화
    if (!m_Model->Initialize(m_Direct3D->GetDevice(), a, dds2,dds1))
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }
    // 멀티 텍스처 쉐이더 객체 생성
    m_MultiTextureShader = new MultiTextureShaderClass;
    if (!m_MultiTextureShader)
        return false;

    // 멀티 텍스처 쉐이더 객체 초기화
    if (!m_MultiTextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the multitexture shader object.", L"Error", MB_OK);
        return false;
    }
    WCHAR c[] = L"../Project1/data/seafloor.dds";
    m_Box = new BoxClass;
    if (!m_Box->Initialize(m_Direct3D->GetDevice(), c, hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }
    
    m_Bitmap = new BitmapClass;

    if (!m_Bitmap)
    {
        return false;
    }

    WCHAR bit[] = L"../Project1/data/seafloor.dds";

    // 비트맵 객체 초기화
    if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, bit,
        256, 256))
    {
        MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
        return false;
    }

    // m_TextureShader 객체 생성
    m_texture = new TextureShaderClass;
    if (!m_texture)
    {
        return false;
    }

    // m_TextureShader 객체 초기화
    if (!m_texture->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
        return false;
    }

    // m_LightShader 객체 생성
    m_LightShader = new LightShaderClass;
    if (!m_LightShader)
    {
        return false;
    }

    // m_LightShader 객체 초기화
    if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
        return false;
    }
    // m_ColorShader 객체 생성
    m_ColorShader = new ColorShaderClass;
    if (!m_ColorShader)
    {
        return false;
    }

    // m_ColorShader 객체 초기화
    if (!m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
        return false;
    }
    // m_Light 객체 생성
    m_Light = new LightClass;
    if (!m_Light)
    {
        return false;
    }
    m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_Light->SetDirection(0.0f, -0.3f, 1.0f);
    m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_Light->SetSpecularPower(32.0f);

  
    m_ModelList = new ModelListClass;
    // 모델 목록 객체 초기화
    if (!m_ModelList->Initialize(1000))
    {
        MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
        return false;
    }

    // 프러스텀 객체 생성
    m_Frustum = new FrustumClass;
    if (!m_Frustum)
    {
        return false;
    }

    // 렌더링 텍스처 객체를 생성한다.
    m_RenderTexture = new RenderTextureClass;
    if (!m_RenderTexture)
    {
        return false;
    }

    // 렌더링 텍스처 객체를 초기화한다.
    if (!m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight))
    {
        return false;
    }

    // 디버그 창 객체를 만듭니다.
    m_DebugWindow = new DebugWindowClass;
    if (!m_DebugWindow)
    {
        return false;
    }

    // 디버그 창 객체를 초기화 합니다.
    if (!m_DebugWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, 256, 256))
    {
        MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
        return false;
    }

    m_Grid = new GridClass;
    if (!m_Grid)
    {
        return false;
    }
    WCHAR GridMesh[] = L"../Project1/data/dirt01.dds";
    if (!m_Grid->Initialize(m_Direct3D->GetDevice(), GridMesh, hwnd))
    {
        return false;
    }
    return true;
}


void GraphicsClass::Shutdown()
{
    // 멀티 텍스처 쉐이더 객체 반환
    if (m_MultiTextureShader)
    {
        m_MultiTextureShader->Shutdown();
        delete m_MultiTextureShader;
        m_MultiTextureShader = 0;
    }
    // 디버그 창 객체를 해제합니다.
    if (m_DebugWindow)
    {
        m_DebugWindow->Shutdown();
        delete m_DebugWindow;
        m_DebugWindow = 0;
    }

    // 렌더를 텍스쳐 객체로 릴리즈한다.
    if (m_RenderTexture)
    {
        m_RenderTexture->Shutdown();
        delete m_RenderTexture;
        m_RenderTexture = 0;
    }
    // m_Bitmap 객체 반환
    if (m_Bitmap)
    {
        m_Bitmap->Shutdown();
        delete m_Bitmap;
        m_Bitmap = 0;
    }

    // m_TextureShader 객체 반환
    if (m_texture)
    {
        m_texture->Shutdown();
        delete m_texture;
        m_texture = 0;
    }
    // m_Text 객체 반환
    if (m_Text)
    {
        m_Text->Shutdown();
        delete m_Text;
        m_Text = 0;
    }
    if (m_Box)
    {
        m_Box->Shutdown();
        delete m_Box;
        m_Box = 0;
    }
    // m_Light 객체 반환
    if (m_Light)
    {
        delete m_Light;
        m_Light = 0;
    }

    // m_LightShader 객체 반환
    if (m_LightShader)
    {
        m_LightShader->Shutdown();
        delete m_LightShader;
        m_LightShader = 0;
    }

    // m_Model 객체 반환
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }

    // m_Camera 객체 반환
    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = 0;
    }

    // Direct3D 객체 반환
    if (m_Direct3D)
    {
        m_Direct3D->Shutdown();
        delete m_Direct3D;
        m_Direct3D = 0;
    }
    // 프러스텀 객체 반환
    if (m_Frustum)
    {
        delete m_Frustum;
        m_Frustum = 0;
    }

    // 모델 목록 객체 반환
    if (m_ModelList)
    {
        m_ModelList->Shutdown();
        delete m_ModelList;
        m_ModelList = 0;
    }
}


bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime,float rotationY,float positionX,float PositionZ)
{
    // 초당 프레임 수를 설정합니다.
    if (!m_Text->SetFps(fps, m_Direct3D->GetDeviceContext()))
    {
        return false;
    }

    // cpu 사용을 설정합니다.
    if (!m_Text->SetCpu(cpu, m_Direct3D->GetDeviceContext()))
    {
        return false;
    }
    // 카메라 위치 설정
   // m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

    m_rotationY += rotationY;
 

    m_positionX += positionX;
    m_positionZ += PositionZ;
    m_Camera->SetPosition(m_positionX, m_positionY, m_positionZ);
    m_Camera->SetRotation(m_rotationX, m_rotationY, m_rotationZ);
    //static float rotation = 0.0f;
    //if (!m_Text->SetMousePosition(mouseX, mouseY, m_Direct3D->GetDeviceContext()))
    //{
    //    return false;
    //}
    //// 각 프레임의 rotation 변수를 업데이트합니다.
    //rotation += (float)XM_PI * 0.01f;
    //if (rotation > 360.0f)
    //{
    //    rotation -= 360.0f;
    //}

    //// 그래픽 랜더링 처리
    //return Render(rotation);

    return true;
}


bool GraphicsClass::Render(float rotation)
{
    // 전체 장면을 먼저 텍스처로 렌더링합니다.
   /* if (!RenderToTexture())
    {
        return false;
    }*/
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // 씬을 그리기 위해 버퍼를 지웁니다
    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // 백 버퍼의 장면을 정상적으로 렌더링합니다.
    //if (!RenderScene())
    //{
    //    return false;
    //}

    // 카메라의 위치에 따라 뷰 행렬을 생성합니다
    m_Camera->Render();
  
    // 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;


    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);
    m_Direct3D->GetOrthoMatrix(orthoMatrix);
 ///*   XMMATRIX Rot = XMMatrixRotationX(0);
 //   XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 2.0f);
 //   XMMATRIX Tr = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

 //   worldMatrix = scale * Tr * Rot;*/
    // 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
    if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 0, 0))
    {
        return false;
    }

    // 텍스처 쉐이더로 비트 맵을 렌더링합니다.    
    if (!m_texture->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix,
        orthoMatrix, m_Bitmap->GetTexture()))
    {
        return false;
    }
    m_Direct3D->TurnZBufferOff();
    // Turn on the alpha blending before rendering the text.
    m_Direct3D->TurnOnAlphaBlending();

    // Render the text strings.
 

    //// 비트 맵 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
    //if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 0, 0))
    //{
    //    return false;
    //}

    //// 텍스처 쉐이더로 비트 맵을 렌더링합니다.    
    //if (!m_texture->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix,
    //    orthoMatrix, m_Bitmap->GetTexture()))
    //{
    //    return false;
    //}
        // 디버그 윈도우 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
    //if (!m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), 50, 50))
    //{
    //    return false;
    //}

    //// 텍스처 셰이더를 사용해 디버그 윈도우를 렌더링한다.
    //if (!m_texture->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
    //    orthoMatrix, m_RenderTexture->GetShaderResourceView()))
    //{
    //    return false;
    //}
    // Turn off alpha blending after rendering the text.
    m_Direct3D->TurnOffAlphaBlending();
    // 모든 2D 렌더링이 완료되었으므로 Z 버퍼를 다시 켜십시오.
    m_Direct3D->TurnZBufferOn();
 
   /* // 절두체를 만듭니다.
    m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

    // 렌더링 될 모델의 수를 얻는다.
    int modelCount = m_ModelList->GetModelCount();

    //  렌더링 된 모델의 개수를 초기화합니다.
    int renderCount = 0;
    // 삼각형이 회전 할 수 있도록 회전 값으로 월드 행렬을 회전합니다.
 

    float positionX = 0;
    float positionY = 0;
    float positionZ = 0;
    float radius = 1.0f; // 구의 반지름을 1.0f로 설정
    XMFLOAT4 color;

    //// 모든 모델을 살펴보고 카메라 뷰에서 볼 수 있는 경우에만 렌더링합니다.
    for (int index = 0; index < modelCount; index++)
    {
        //이 인덱스에서 구형 모델의 위치와 색상을 가져옵니다.
        m_ModelList->GetData(index, positionX, positionY, positionZ, color);

        // 구형 모델이 뷰 frustum에 있는지 확인합니다.
        if (m_Frustum->CheckCube(positionX, positionY, positionZ, radius))
        {
            worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

            // 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 드로잉을 준비합니다.
            m_Model->Render(m_Direct3D->GetDeviceContext());

            //// 라이트 쉐이더를 사용하여 모델을 렌더링합니다.
            //m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
            //    projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(),
            //    m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
            //    m_Light->GetSpecularPower());
            m_MultiTextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
                projectionMatrix, m_Model->GetTextureArray());


            // 원래의 월드 매트릭스로 리셋.
            m_Direct3D->GetWorldMatrix(worldMatrix);

            //이 모델이 렌더링 되었기 때문에이 프레임의 수를 늘립니다.
            renderCount++;
        }
    }
    // 모델을 렌더링 할 위치로 이동합니다.
 /*   XMVECTOR Roation = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
  
   
  
  /*  worldMatrix = scale * Tr * Rot;*/
    static float TrOffset[3] = { 0,0,0 };
    static float RtOffset[3] = { 0,0,0};
    static float ScOffset[3] = { 1, 1, 1 };
    XMVECTOR Rotvertor = { RtOffset[0],RtOffset[1],RtOffset[2] };
    XMMATRIX RotX = XMMatrixRotationX(XMConvertToRadians(RtOffset[0]));
    XMMATRIX RotY = XMMatrixRotationY(XMConvertToRadians(RtOffset[1]));
    XMMATRIX RotZ = XMMatrixRotationZ(XMConvertToRadians(RtOffset[2]));
    XMMATRIX scale = XMMatrixScaling(ScOffset[0], ScOffset[1], ScOffset[2]);
    XMMATRIX Tr = XMMatrixTranslation(TrOffset[0], TrOffset[1], TrOffset[2]);
    worldMatrix = scale* RotX * RotY * RotZ  * Tr  ;
   
    m_Box->Render(m_Direct3D->GetDeviceContext());

  /*  if(m_texture->Render(m_Direct3D->GetDeviceContext(),m_Box->GetIndexCount(),worldMatrix,viewMatrix,projectionMatrix,m_Box->GetTexture()));*/

    if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Box->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,m_Box->GetTexture() ,m_Light->GetDirection(), m_Light->GetAmbientColor(),
        m_Light->GetDiffuseColor(),m_Camera->GetPosition() , m_Light->GetSpecularColor(), m_Light->GetSpecularPower()));
    {
    }
    m_Direct3D->GetWorldMatrix(worldMatrix);

    XMMATRIX GridTr = XMMatrixTranslation(0.0, -3.0, 0.0);
    worldMatrix = GridTr;
    m_Grid->Render(m_Direct3D->GetDeviceContext());

    if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Grid->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Grid->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(),
        m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower()));
    {
    }
    m_Direct3D->TurnZBufferOff();
    m_Direct3D->TurnOnAlphaBlending();
    if (!m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix))
    {
        return false;
    }
    m_Direct3D->TurnOffAlphaBlending();
    m_Direct3D->TurnZBufferOn();
    //worldMatrix = XMMatrixRotationY(rotation);
    //// 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 드로잉을 준비합니다.
    //m_Model->Render(m_Direct3D->GetDeviceContext());
    //
    //// Light 쉐이더를 사용하여 모델을 렌더링합니다.
    //if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
    //    projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(),
    //    m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
    //    m_Light->GetSpecularPower()))
    //{
    //    return false;
    //}
    
    // 버퍼의 내용을 화면에 출력합니다

    static int counter = 0;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test");
    ImGui::Text("Example Test");
    if (ImGui::Button("Click Me!"))
        counter += 1;
    ImGui::SameLine();
    std::string clickCount = "Click Count: " + std::to_string(counter);
    ImGui::Text(clickCount.c_str());
    ImGui::DragFloat3("BoxTr", TrOffset, 0.1f, -5.0f, 5.0f);
    ImGui::DragFloat3("BoxRt", RtOffset, 5, 0, 360);
    ImGui::DragFloat3("BoxSc", ScOffset, 0.1f, 0.1f, 20);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());





    m_Direct3D->EndScene();

    return true;
}
bool GraphicsClass::RenderToTexture()
{
    // 렌더링 대상을 렌더링에 맞게 설정합니다.
    m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

    // 렌더링을 텍스처에 지웁니다.
    m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f,
        1.0f);

    // 이제 장면을 렌더링하면 백 버퍼 대신 텍스처로 렌더링됩니다.
    if (!RenderScene())
    {
        return false;
    }

    // 렌더링 대상을 원래의 백 버퍼로 다시 설정하고 렌더링에 대한 렌더링을 더 이상 다시 설정하지 않습니다.
    m_Direct3D->SetBackBufferRenderTarget();

    return true;
}


bool GraphicsClass::RenderScene()
{
    // 카메라의 위치에 따라 뷰 행렬을 생성합니다
    m_Camera->Render();

    // 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);
    m_Direct3D->GetOrthoMatrix(orthoMatrix);

    // 각 프레임의 rotation 변수를 업데이트합니다.
    static float rotation = 0.0f;
    rotation += (float)XM_PI * 0.0025f;
    if (rotation > 360.0f)
    {
        rotation -= 360.0f;
    }

    // 회전 값으로 월드 행렬을 회전합니다.
    worldMatrix = XMMatrixRotationY(rotation);

    // 모델 버텍스와 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 렌더링 합니다.
    m_Model->Render(m_Direct3D->GetDeviceContext());

    // 라이트 쉐이더를 사용하여 모델을 렌더링합니다.
    return m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
        projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(),
        m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
        m_Light->GetSpecularPower());

}
