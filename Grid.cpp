#include "stdafx.h"
#include "GeometryGenerator.h"
#include "TextureClass.h"
#include "Grid.h"
#include <fstream>

GridClass::GridClass()
{
}

GridClass::GridClass(const GridClass&)
{
}

GridClass::~GridClass()
{
}

bool GridClass::Initialize(ID3D11Device* device, WCHAR* b, HWND hwnd)
{
	//// �� �����͸� �ε��մϴ�.
 //if (!LoadModel(b))
 //{
 //    return false;
 //}
	
	if (b != nullptr)
	{
		if (!InitializeBuffers(device));
		return LoadTexture(device, hwnd, b);
	}
	else 
		return !InitializeBuffers(device);
}

void GridClass::Shutdown()
{
	// ���ؽ� �� �ε��� ���۸� �����մϴ�.
	ShutdownBuffers();

	ReleaseModel();
}

void GridClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int GridClass::GetIndexCount()
{
	return m_IC;;
}

ID3D11ShaderResourceView* GridClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool GridClass::InitializeBuffers(ID3D11Device* device)
{
    GeometryGenerator mesh;
    GeometryGenerator::MeshData grid = mesh.CreateGrid(80, 80, 2,2);

    // ���� �迭�� ����ϴ�.
    std::vector<VertexType> vertices(grid.Vertices.size());


    // �ε��� �迭�� ����ϴ�.
    std::vector<UINT> indices;
    indices.insert(indices.end(), grid.Indices32.begin(), grid.Indices32.end());

    // ���� �迭�� �ε��� �迭�� �����ͷ� �о�ɴϴ�.
    for (size_t i = 0; i < grid.Vertices.size(); i++)
    {
        vertices[i].position = grid.Vertices[i].Position;
        vertices[i].texture = grid.Vertices[i].TexC;
        vertices[i].normal = grid.Vertices[i].Normal;

    }

    // ���� ���� ������ ����ü�� �����մϴ�.
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * grid.Vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = &vertices[0];
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
    indexBufferDesc.ByteWidth = sizeof(UINT) * grid.Indices32.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = &indices[0];
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;
    m_IC = grid.Indices32.size();
    // �ε��� ���۸� �����մϴ�.
    if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
    {
        return false;
    }

    // �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.


    return true;
}

void GridClass::ShutdownBuffers()
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

void GridClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool GridClass::LoadTexture(ID3D11Device* device, HWND hwnd, WCHAR* Filename1)
{
    // �ؽ�ó ������Ʈ�� �����Ѵ�.
    m_Texture = new TextureClass;
    if (!m_Texture)
    {
        return false;
    }

    // �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
    m_Texture->Initialize(device, Filename1);
}

void GridClass::ReleaseTexture()
{
    m_Texture->Shutdown();
}

bool GridClass::LoadModel(char* filename)
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

void GridClass::ReleaseModel()
{
    if (m_model)
    {
        delete[] m_model;
        m_model = 0;
    }
}
