#pragma once

class TextureClass;
class TextureArrayClass;

class ModelClass : public AlignedAllocationPolicy<16>
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };
    struct ModelType
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };
public:
    ModelClass();
    ModelClass(const ModelClass&);
    ~ModelClass();

    bool Initialize(ID3D11Device*,char*, WCHAR*, WCHAR*);
    void Shutdown();
    void Render(ID3D11DeviceContext*);

    int GetIndexCount();
    ID3D11ShaderResourceView* GetTexture();
    ID3D11ShaderResourceView** GetTextureArray();

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext*);

    bool LoadTexture(ID3D11Device*, WCHAR*);
    void ReleaseTexture();

    bool LoadTextureArray(ID3D11Device*, WCHAR*, WCHAR*);
    void ReleaseTextureArray();

    bool LoadModel(char*);
    void ReleaseModel();
private:
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;
    int m_vertexCount = 0;
    int m_indexCount = 0;
    TextureClass* m_Texture = nullptr;
    TextureArrayClass* m_TextureArray = nullptr;
    ModelType* m_model = nullptr;
};
