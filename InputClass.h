#pragma once

class InputClass
{
public:
    InputClass();
    InputClass(const InputClass&);
    ~InputClass();

    bool Initialize(HINSTANCE, HWND, int, int);

    void KeyDown(unsigned int);
    void KeyUp(unsigned int);
    void Shutdown();
    bool Frame();
    bool IsLeftArrowPressed();
    bool IsRightArrowPressed();
    bool IsEscapePressed();
    void GetMouseLocation(int&, int&);
    bool IsKeyDown(unsigned int);

private:
    bool ReadKeyboard();
    bool ReadMouse();
    void ProcessInput();

private:
    IDirectInput8W* m_directInput = nullptr;
    IDirectInputDevice8W* m_keyboard = nullptr;
    IDirectInputDevice8W* m_mouse = nullptr;

    unsigned char m_keyboardState[256] = { 0, };
    DIMOUSESTATE m_mouseState;
    
    

    int m_screenWidth = 0;
    int m_screenHeight = 0;
    int m_mouseX = 0;
    int m_mouseY = 0;
};
