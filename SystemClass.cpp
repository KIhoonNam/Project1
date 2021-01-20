#include "Stdafx.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "FpsClass.h"
#include "CpuClass.h"
#include "PositionClass.h"
#include "TimerClass.h"
#include "SystemClass.h"

SystemClass::SystemClass()
{
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
    // ������ â ����, ���� ���� ���� �ʱ�ȭ
    int screenWidth = 0;
    int screenHeight = 0;
    // ������ ���� �ʱ�ȭ
    InitializeWindows(screenWidth, screenHeight);

    // m_Input ��ü ����. �� Ŭ������ ���� ������� Ű���� �Է� ó���� ���˴ϴ�.
    m_Input = new InputClass;
    if (!m_Input)
    {
        return false;
    }

    // m_Input ��ü �ʱ�ȭ
    if (!m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight))
    {
        MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
        return false;
    }


    // m_Graphics ��ü ����.  �׷��� �������� ó���ϱ� ���� ��ü�Դϴ�.
    m_Graphics = new GraphicsClass;
    if (!m_Graphics)
    {
        return false;
    }

    // m_Graphics ��ü �ʱ�ȭ.
    if (!m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd))
    {
        return false;
    }

    // fps ��ü�� ����ϴ�.
    m_Fps = new FpsClass;
    if (!m_Fps)
    {
        return false;
    }

    // fps ��ü�� �ʱ�ȭ�մϴ�.
    m_Fps->Initialize();

    // cpu ��ü�� ����ϴ�.
    m_Cpu = new CpuClass;
    if (!m_Cpu)
    {
        return false;
    }

    // cpu ��ü�� �ʱ�ȭ�մϴ�.
    m_Cpu->Initialize();

    // Ÿ�̸� ��ü�� ����ϴ�.
    m_Timer = new TimerClass;
    if (!m_Timer)
    {
        return false;
    }

    // Ÿ�̸� ��ü�� �ʱ�ȭ�մϴ�.
    if (!m_Timer->Initialize())
    {
        MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
        return false;
    }

    // ��ġ ��ü�� �����մϴ�.
    m_Position = new PositionClass;
    if (!m_Position)
    {
        return false;
    }
    return true;
}


void SystemClass::Shutdown()
{ 
    // ��ġ ��ü ��ȯ
    if (m_Position)
    {
        delete m_Position;
        m_Position = 0;
    }

    // Ÿ�̸� ��ü�� �����մϴ�.
    if (m_Timer)
    {
        delete m_Timer;
        m_Timer = 0;
    }

    // cpu ��ü�� �����մϴ�.
    if (m_Cpu)
    {
        m_Cpu->Shutdown();
        delete m_Cpu;
        m_Cpu = 0;
    }

    // fps ��ü�� �����մϴ�.
    if (m_Fps)
    {
        delete m_Fps;
        m_Fps = 0;
    }
    // m_Graphics ��ü ��ȯ
    if (m_Graphics)
    {
        m_Graphics->Shutdown();
        delete m_Graphics;
        m_Graphics = 0;
    }

    // m_Input ��ü ��ȯ
    if (m_Input)
    {
        m_Input->Shutdown();
        delete m_Input;
        m_Input = 0;
    }
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Window ���� ó��
    ShutdownWindows();
}


void SystemClass::Run()
{
    // �޽��� ����ü ���� �� �ʱ�ȭ
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
 
   
    // ����ڷκ��� ���� �޽����� ���������� �޽��������� ���ϴ�
    while (true)
    {



        // ������ �޽����� ó���մϴ�
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // ���� �޽����� ���� ��� �޽��� ������ Ż���մϴ�
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // �� �ܿ��� Frame �Լ��� ó���մϴ�.
            if (!Frame())
            {
                MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
                break;
            }
        }
        // ����ڰ� ESCŰ�� �������� Ȯ�� �� ���� ó����
        if (m_Input->IsEscapePressed() == true)
        {
            break;
        }
    }
 
}


bool SystemClass::Frame()
{     
    // �Է� ������ ó���� �����մϴ�
    if (!m_Input->Frame())
    {
        return false;
    }

    // ������Ʈ �� ��ġ�� ����ϱ� ���� ������ �ð��� �����մϴ�.
    m_Position->SetFrameTime(m_Timer->GetTime());

    float mouseMove = m_Input->IsRotation();
    m_Position->TurnLeft(mouseMove);
    m_Position->TurnRight(mouseMove);
    // ���� �Ǵ� ������ ȭ��ǥ Ű�� �������� Ȯ���Ͻʽÿ�. �׷��� ������ ī�޶� ������ ȸ���Ͻʽÿ�.
 
 

   bool keyDown = m_Input->IsWPressed();
    m_Position->MoveFront(keyDown);

    keyDown = m_Input->IsSPressed();
    m_Position->MoveBack(keyDown);

    keyDown = m_Input->IsAPressed();
    m_Position->MoveLeft(keyDown);
    
    keyDown = m_Input->IsDPressed();
    m_Position->MoveRight(keyDown);

    float positionX = 0.0f;
    float positionZ = 0.0f;

    positionX = m_Position->GetPosX();
    positionZ = m_Position->GetPosZ();

    float rotationY = 0;
    m_Position->GetRotation(rotationY);


    // �ý��� ��踦 ������Ʈ �մϴ�
    m_Timer->Frame();
    m_Fps->Frame();
    m_Cpu->Frame();
    int mouseX = 0, mouseY = 0;

  

    // �Է� ��ü���� ���콺 ��ġ�� �����ɴϴ�
    m_Input->GetMouseLocation(mouseX, mouseY);

    // �׷��� ��ü�� Frame�� ó���մϴ�
    if (!m_Graphics->Frame(mouseX, mouseY, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime(),rotationY,positionX,positionZ))
    {
        return false;
    }

    return m_Graphics->Render((float)XM_PI * 0.01f);

}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam))
        return true;
    return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
    // �ܺ� �����͸� �� ��ü�� �����մϴ�
    ApplicationHandle = this;

    // �� ���α׷��� �ν��Ͻ��� �����ɴϴ�
    m_hinstance = GetModuleHandle(NULL);

    // ���α׷� �̸��� �����մϴ�
    m_applicationName = L"Dx11Demo_02";

    // windows Ŭ������ �Ʒ��� ���� �����մϴ�.
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = m_applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    // windows class�� ����մϴ�
    RegisterClassEx(&wc);

    // ����� ȭ���� �ػ󵵸� �о�ɴϴ�
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int posX = 0;
    int posY = 0;

    // FULL_SCREEN ���� ���� ���� ȭ���� �����մϴ�.
    if (FULL_SCREEN)
    {
        // Ǯ��ũ�� ���� �����ߴٸ� ����� ȭ�� �ػ󵵸� ����ũ�� �ػ󵵷� �����ϰ� ������ 32bit�� �����մϴ�.
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Ǯ��ũ������ ���÷��� ������ �����մϴ�.
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
    }
    else
    {
        // ������ ����� ��� 800 * 600 ũ�⸦ �����մϴ�.
        screenWidth = 1920;
        screenHeight = 1080;

        // ������ â�� ����, ������ �� ��� ������ �մϴ�.
        posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }

    // �����츦 �����ϰ� �ڵ��� ���մϴ�.
    m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

    // �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �����մϴ�
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);
}


void SystemClass::ShutdownWindows()
{
    // Ǯ��ũ�� ��忴�ٸ� ���÷��� ������ �ʱ�ȭ�մϴ�.
    if (FULL_SCREEN)
    {
        ChangeDisplaySettings(NULL, 0);
    }

    // â�� �����մϴ�
    DestroyWindow(m_hwnd);
    m_hwnd = NULL;

    // ���α׷� �ν��Ͻ��� �����մϴ�
    UnregisterClass(m_applicationName, m_hinstance);
    m_hinstance = NULL;

    // �ܺ������� ������ �ʱ�ȭ�մϴ�
    ApplicationHandle = NULL;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    switch (umessage)
    {
        // ������ ���Ḧ Ȯ���մϴ�
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    // �����찡 �������� Ȯ���մϴ�
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }

    // �� ���� ��� �޽������� �ý��� Ŭ������ �޽��� ó���� �ѱ�ϴ�.
    default:
    {
        return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
    }
    }
}