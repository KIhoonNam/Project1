#pragma once
#pragma once

class TextureClass;
class ColorShaderClass;


class GridClass : public AlignedAllocationPolicy<16>
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
        XMFLOAT3 normal;
    };
    struct ModelType
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };
public:
    GridClass();
    GridClass(const GridClass&);
    ~GridClass();

    bool Initialize(ID3D11Device*, WCHAR*, HWND);
    void Shutdown();
    void Render(ID3D11DeviceContext*);

    int GetIndexCount();
    ID3D11ShaderResourceView* GetTexture();

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext*);

    bool LoadTexture(ID3D11Device*, HWND, WCHAR*);
    void ReleaseTexture();

    bool LoadModel(char*);
    void ReleaseModel();
private:
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;
    int m_VC;
    int m_IC;
    TextureClass* m_Texture = nullptr;
    ColorShaderClass* m_Color = nullptr;
    ModelType* m_model = nullptr;
};
