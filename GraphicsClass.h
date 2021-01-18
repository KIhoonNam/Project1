/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class CameraClass;
class ModelClass;
class LightClass;
class LightShaderClass;
class ColorShaderClass;
class BoxClass;
class BitmapClass;
class TextureShaderClass;
class TextClass;
class ModelListClass;
class FrustumClass;
class MultiTextureShaderClass;
class RenderTextureClass;
class DebugWindowClass;

class GraphicsClass
{
public:
    GraphicsClass();
    GraphicsClass(const GraphicsClass&);
    ~GraphicsClass();

    bool Initialize(int, int, HWND);
    void Shutdown();
    bool Frame(int,int,int,int,float,float);
    bool Render(float);

private:
    bool RenderToTexture();
    bool RenderScene();

private:

    float m_rotationY = 0;

    D3DClass* m_Direct3D = nullptr;
    CameraClass* m_Camera = nullptr;
    CameraClass* m_Camera1 = nullptr;
    ModelClass* m_Model = nullptr;
    LightShaderClass* m_LightShader = nullptr;
    LightClass* m_Light = nullptr;
    ColorShaderClass* m_ColorShader = nullptr;
    BoxClass* m_Box = nullptr;
    BitmapClass* m_Bitmap = nullptr;
    TextureShaderClass* m_texture = nullptr;
    TextClass* m_Text = nullptr;
    ModelListClass* m_ModelList = nullptr;
    FrustumClass* m_Frustum = nullptr;
    RenderTextureClass* m_RenderTexture = nullptr;
    DebugWindowClass* m_DebugWindow = nullptr;
    MultiTextureShaderClass* m_MultiTextureShader = nullptr;
};
