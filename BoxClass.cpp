#include "stdafx.h"
#include "TextureClass.h"
#include "Boxclass.h"
#include <fstream>


BoxClass::BoxClass()
{
    m_VC = 0;
    m_IC = 0;
}


BoxClass::BoxClass(const BoxClass& other)
{
    m_VC = 0;
    m_IC = 0;
}


BoxClass::~BoxClass()
{
}


bool BoxClass::Initialize(ID3D11Device* device,char* b,HWND hwnd)
{
    // �� �����͸� �ε��մϴ�.
    if (!LoadModel(b))
    {
        return false;
    }

    return InitializeBuffers(device);
}


void BoxClass::Shutdown()
{


    // ���ؽ� �� �ε��� ���۸� �����մϴ�.
    ShutdownBuffers();

    ReleaseModel();
}


void BoxClass::Render(ID3D11DeviceContext* deviceContext)
{
    // �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� �����ϴ�.
    RenderBuffers(deviceContext);
}


int BoxClass::GetIndexCount()
{
    return m_IC;
}


ID3D11ShaderResourceView* BoxClass::GetTexture()
{
    return nullptr;
}


bool BoxClass::InitializeBuffers(ID3D11Device* device)
{


    // ���� �迭�� ����ϴ�.
    VertexType* vertices = new VertexType[m_VC];
    if (!vertices)
    {
        return false;
    }

    // �ε��� �迭�� ����ϴ�.
    unsigned long* indices = new unsigned long[m_IC];
    if (!indices)
    {
        return false;
    }

    // ���� �迭�� �ε��� �迭�� �����ͷ� �о�ɴϴ�.
    for (int i = 0; i < m_VC; i++)
    {
        vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
        vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
        vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

        indices[i] = i;
    }

    // ���� ���� ������ ����ü�� �����մϴ�.
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VC;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // ���� ���� ���۸� ����ϴ�.
    if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
    {
        return false;
    }

    // ���� �ε��� ������ ����ü�� �����մϴ�.
    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IC;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // �ε��� ���۸� �����մϴ�.
    if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
    {
        return false;
    }

    // �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
    delete[] vertices;
    vertices = 0;

    delete[] indices;
    indices = 0;

    return true;
}


void BoxClass::ShutdownBuffers()
{
    // �ε��� ���۸� �����մϴ�.
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    // ���� ���۸� �����մϴ�.
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }
}


void BoxClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    // ���� ������ ������ �������� �����մϴ�.
    UINT stride = sizeof(VertexType);
    UINT offset = 0;

    // ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool BoxClass::LoadTexture(ID3D11Device* device, HWND hwnd)
{
    // �ؽ�ó ������Ʈ�� �����Ѵ�.
    m_Texture = new TextureClass;
    if (!m_Texture)
    {
        return false;
    }

    // �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
    
}

bool BoxClass::LoadModel(char* filename)
{
    // �� ������ ���ϴ�.
    std::ifstream fin;
    fin.open(filename);

    // ������ �� �� ������ �����մϴ�.
    if (fin.fail())
    {
        return false;
    }

    // ���ؽ� ī��Ʈ�� ������ �д´�.
    char input = 0;
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    int Vertex = 0;
    fin >> Vertex;
    // ���ؽ� ī��Ʈ�� �д´�.
    m_VC = Vertex;

    // �ε����� ���� ���� ���� ���� �����մϴ�.
    m_IC = m_VC;

    // �о� ���� ���� ������ ����Ͽ� ���� ����ϴ�.
    m_model = new ModelType[m_VC];
    if (!m_model)
    {
        return false;
    }

    // �������� ���� �κб��� �д´�.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    // ���ؽ� �����͸� �н��ϴ�.
    for (int i = 0; i < m_VC; i++)
    {
        fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
        fin >> m_model[i].tu >> m_model[i].tv;
        fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
    }

    // �� ������ �ݴ´�.
    fin.close();

    return true;
}


void BoxClass::ReleaseModel()
{
    if (m_model)
    {
        delete[] m_model;
        m_model = 0;
    }
}
