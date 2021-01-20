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
    // Direct3D ��ü ����
    m_Direct3D = new D3DClass;
    if (!m_Direct3D)
    {
        return false;
    }

    // Direct3D ��ü �ʱ�ȭ
    if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
    {
        MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
        return false;
    }  // m_Camera ��ü ����
    m_Camera = new CameraClass;
    if (!m_Camera)
    {
        return false;
    }
    XMMATRIX baseViewMatrix;
    m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
    m_Camera->Render();
    m_Camera->GetViewMatrix(baseViewMatrix);
    // m_Text ��ü ����
    m_Text = new TextClass;
    if (!m_Text)
    {
        return false;
    }

    // m_Text ��ü �ʱ�ȭ
    if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight,
        baseViewMatrix))
    {
        MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
        return false;
    }
  

    m_Camera->SetPosition(m_positionX, m_positionY, m_positionZ);
    m_Camera->SetRotation(m_rotationX, m_rotationY, m_rotationZ); 
    // m_Model ��ü ����
    m_Model = new ModelClass;
    if (!m_Model)
    {
        return false;
    }
    char a[] = "../Project1/data/cube.txt";
    WCHAR dds1[] = L"../Project1/data/stone01.dds";
    WCHAR dds2[] = L"../Project1/data/dirt01.dds";
    // m_Model ��ü �ʱ�ȭ
    if (!m_Model->Initialize(m_Direct3D->GetDevice(), a, dds2,dds1))
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }
    // ��Ƽ �ؽ�ó ���̴� ��ü ����
    m_MultiTextureShader = new MultiTextureShaderClass;
    if (!m_MultiTextureShader)
        return false;

    // ��Ƽ �ؽ�ó ���̴� ��ü �ʱ�ȭ
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

    // ��Ʈ�� ��ü �ʱ�ȭ
    if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, bit,
        256, 256))
    {
        MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
        return false;
    }

    // m_TextureShader ��ü ����
    m_texture = new TextureShaderClass;
    if (!m_texture)
    {
        return false;
    }

    // m_TextureShader ��ü �ʱ�ȭ
    if (!m_texture->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
        return false;
    }

    // m_LightShader ��ü ����
    m_LightShader = new LightShaderClass;
    if (!m_LightShader)
    {
        return false;
    }

    // m_LightShader ��ü �ʱ�ȭ
    if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
        return false;
    }
    // m_ColorShader ��ü ����
    m_ColorShader = new ColorShaderClass;
    if (!m_ColorShader)
    {
        return false;
    }

    // m_ColorShader ��ü �ʱ�ȭ
    if (!m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
        return false;
    }
    // m_Light ��ü ����
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
    // �� ��� ��ü �ʱ�ȭ
    if (!m_ModelList->Initialize(1000))
    {
        MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
        return false;
    }

    // �������� ��ü ����
    m_Frustum = new FrustumClass;
    if (!m_Frustum)
    {
        return false;
    }

    // ������ �ؽ�ó ��ü�� �����Ѵ�.
    m_RenderTexture = new RenderTextureClass;
    if (!m_RenderTexture)
    {
        return false;
    }

    // ������ �ؽ�ó ��ü�� �ʱ�ȭ�Ѵ�.
    if (!m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight))
    {
        return false;
    }

    // ����� â ��ü�� ����ϴ�.
    m_DebugWindow = new DebugWindowClass;
    if (!m_DebugWindow)
    {
        return false;
    }

    // ����� â ��ü�� �ʱ�ȭ �մϴ�.
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
    // ��Ƽ �ؽ�ó ���̴� ��ü ��ȯ
    if (m_MultiTextureShader)
    {
        m_MultiTextureShader->Shutdown();
        delete m_MultiTextureShader;
        m_MultiTextureShader = 0;
    }
    // ����� â ��ü�� �����մϴ�.
    if (m_DebugWindow)
    {
        m_DebugWindow->Shutdown();
        delete m_DebugWindow;
        m_DebugWindow = 0;
    }

    // ������ �ؽ��� ��ü�� �������Ѵ�.
    if (m_RenderTexture)
    {
        m_RenderTexture->Shutdown();
        delete m_RenderTexture;
        m_RenderTexture = 0;
    }
    // m_Bitmap ��ü ��ȯ
    if (m_Bitmap)
    {
        m_Bitmap->Shutdown();
        delete m_Bitmap;
        m_Bitmap = 0;
    }

    // m_TextureShader ��ü ��ȯ
    if (m_texture)
    {
        m_texture->Shutdown();
        delete m_texture;
        m_texture = 0;
    }
    // m_Text ��ü ��ȯ
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
    // m_Light ��ü ��ȯ
    if (m_Light)
    {
        delete m_Light;
        m_Light = 0;
    }

    // m_LightShader ��ü ��ȯ
    if (m_LightShader)
    {
        m_LightShader->Shutdown();
        delete m_LightShader;
        m_LightShader = 0;
    }

    // m_Model ��ü ��ȯ
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }

    // m_Camera ��ü ��ȯ
    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = 0;
    }

    // Direct3D ��ü ��ȯ
    if (m_Direct3D)
    {
        m_Direct3D->Shutdown();
        delete m_Direct3D;
        m_Direct3D = 0;
    }
    // �������� ��ü ��ȯ
    if (m_Frustum)
    {
        delete m_Frustum;
        m_Frustum = 0;
    }

    // �� ��� ��ü ��ȯ
    if (m_ModelList)
    {
        m_ModelList->Shutdown();
        delete m_ModelList;
        m_ModelList = 0;
    }
}


bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime,float rotationY,float positionX,float PositionZ)
{
    // �ʴ� ������ ���� �����մϴ�.
    if (!m_Text->SetFps(fps, m_Direct3D->GetDeviceContext()))
    {
        return false;
    }

    // cpu ����� �����մϴ�.
    if (!m_Text->SetCpu(cpu, m_Direct3D->GetDeviceContext()))
    {
        return false;
    }
    // ī�޶� ��ġ ����
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
    //// �� �������� rotation ������ ������Ʈ�մϴ�.
    //rotation += (float)XM_PI * 0.01f;
    //if (rotation > 360.0f)
    //{
    //    rotation -= 360.0f;
    //}

    //// �׷��� ������ ó��
    //return Render(rotation);

    return true;
}


bool GraphicsClass::Render(float rotation)
{
    // ��ü ����� ���� �ؽ�ó�� �������մϴ�.
   /* if (!RenderToTexture())
    {
        return false;
    }*/
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // ���� �׸��� ���� ���۸� ����ϴ�
    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // �� ������ ����� ���������� �������մϴ�.
    //if (!RenderScene())
    //{
    //    return false;
    //}

    // ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
    m_Camera->Render();
  
    // ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;


    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);
    m_Direct3D->GetOrthoMatrix(orthoMatrix);
 ///*   XMMATRIX Rot = XMMatrixRotationX(0);
 //   XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 2.0f);
 //   XMMATRIX Tr = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

 //   worldMatrix = scale * Tr * Rot;*/
    // ��Ʈ �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
    if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 0, 0))
    {
        return false;
    }

    // �ؽ�ó ���̴��� ��Ʈ ���� �������մϴ�.    
    if (!m_texture->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix,
        orthoMatrix, m_Bitmap->GetTexture()))
    {
        return false;
    }
    m_Direct3D->TurnZBufferOff();
    // Turn on the alpha blending before rendering the text.
    m_Direct3D->TurnOnAlphaBlending();

    // Render the text strings.
 

    //// ��Ʈ �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
    //if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 0, 0))
    //{
    //    return false;
    //}

    //// �ؽ�ó ���̴��� ��Ʈ ���� �������մϴ�.    
    //if (!m_texture->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix,
    //    orthoMatrix, m_Bitmap->GetTexture()))
    //{
    //    return false;
    //}
        // ����� ������ ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� �׸��⸦ �غ��մϴ�.
    //if (!m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), 50, 50))
    //{
    //    return false;
    //}

    //// �ؽ�ó ���̴��� ����� ����� �����츦 �������Ѵ�.
    //if (!m_texture->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
    //    orthoMatrix, m_RenderTexture->GetShaderResourceView()))
    //{
    //    return false;
    //}
    // Turn off alpha blending after rendering the text.
    m_Direct3D->TurnOffAlphaBlending();
    // ��� 2D �������� �Ϸ�Ǿ����Ƿ� Z ���۸� �ٽ� �ѽʽÿ�.
    m_Direct3D->TurnZBufferOn();
 
   /* // ����ü�� ����ϴ�.
    m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

    // ������ �� ���� ���� ��´�.
    int modelCount = m_ModelList->GetModelCount();

    //  ������ �� ���� ������ �ʱ�ȭ�մϴ�.
    int renderCount = 0;
    // �ﰢ���� ȸ�� �� �� �ֵ��� ȸ�� ������ ���� ����� ȸ���մϴ�.
 

    float positionX = 0;
    float positionY = 0;
    float positionZ = 0;
    float radius = 1.0f; // ���� �������� 1.0f�� ����
    XMFLOAT4 color;

    //// ��� ���� ���캸�� ī�޶� �信�� �� �� �ִ� ��쿡�� �������մϴ�.
    for (int index = 0; index < modelCount; index++)
    {
        //�� �ε������� ���� ���� ��ġ�� ������ �����ɴϴ�.
        m_ModelList->GetData(index, positionX, positionY, positionZ, color);

        // ���� ���� �� frustum�� �ִ��� Ȯ���մϴ�.
        if (m_Frustum->CheckCube(positionX, positionY, positionZ, radius))
        {
            worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

            // �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������� �غ��մϴ�.
            m_Model->Render(m_Direct3D->GetDeviceContext());

            //// ����Ʈ ���̴��� ����Ͽ� ���� �������մϴ�.
            //m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
            //    projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(),
            //    m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
            //    m_Light->GetSpecularPower());
            m_MultiTextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
                projectionMatrix, m_Model->GetTextureArray());


            // ������ ���� ��Ʈ������ ����.
            m_Direct3D->GetWorldMatrix(worldMatrix);

            //�� ���� ������ �Ǿ��� �������� �������� ���� �ø��ϴ�.
            renderCount++;
        }
    }
    // ���� ������ �� ��ġ�� �̵��մϴ�.
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
    //// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������� �غ��մϴ�.
    //m_Model->Render(m_Direct3D->GetDeviceContext());
    //
    //// Light ���̴��� ����Ͽ� ���� �������մϴ�.
    //if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
    //    projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(),
    //    m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
    //    m_Light->GetSpecularPower()))
    //{
    //    return false;
    //}
    
    // ������ ������ ȭ�鿡 ����մϴ�

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
    // ������ ����� �������� �°� �����մϴ�.
    m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());

    // �������� �ؽ�ó�� ����ϴ�.
    m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f,
        1.0f);

    // ���� ����� �������ϸ� �� ���� ��� �ؽ�ó�� �������˴ϴ�.
    if (!RenderScene())
    {
        return false;
    }

    // ������ ����� ������ �� ���۷� �ٽ� �����ϰ� �������� ���� �������� �� �̻� �ٽ� �������� �ʽ��ϴ�.
    m_Direct3D->SetBackBufferRenderTarget();

    return true;
}


bool GraphicsClass::RenderScene()
{
    // ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
    m_Camera->Render();

    // ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);
    m_Direct3D->GetOrthoMatrix(orthoMatrix);

    // �� �������� rotation ������ ������Ʈ�մϴ�.
    static float rotation = 0.0f;
    rotation += (float)XM_PI * 0.0025f;
    if (rotation > 360.0f)
    {
        rotation -= 360.0f;
    }

    // ȸ�� ������ ���� ����� ȸ���մϴ�.
    worldMatrix = XMMatrixRotationY(rotation);

    // �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������ �մϴ�.
    m_Model->Render(m_Direct3D->GetDeviceContext());

    // ����Ʈ ���̴��� ����Ͽ� ���� �������մϴ�.
    return m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix,
        projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(),
        m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
        m_Light->GetSpecularPower());

}
