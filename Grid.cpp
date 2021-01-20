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
	//// 모델 데이터를 로드합니다.
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
	// 버텍스 및 인덱스 버퍼를 종료합니다.
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

    // 정점 배열을 만듭니다.
    std::vector<VertexType> vertices(grid.Vertices.size());


    // 인덱스 배열을 만듭니다.
    std::vector<UINT> indices;
    indices.insert(indices.end(), grid.Indices32.begin(), grid.Indices32.end());

    // 정점 배열과 인덱스 배열을 데이터로 읽어옵니다.
    for (size_t i = 0; i < grid.Vertices.size(); i++)
    {
        vertices[i].position = grid.Vertices[i].Position;
        vertices[i].texture = grid.Vertices[i].TexC;
        vertices[i].normal = grid.Vertices[i].Normal;

    }

    // 정적 정점 버퍼의 구조체를 설정합니다.
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * grid.Vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = &vertices[0];
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // 이제 정점 버퍼를 만듭니다.
    if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
    {
        return false;
    }

    // 정적 인덱스 버퍼의 구조체를 설정합니다.
    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(UINT) * grid.Indices32.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = &indices[0];
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;
    m_IC = grid.Indices32.size();
    // 인덱스 버퍼를 생성합니다.
    if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
    {
        return false;
    }

    // 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.


    return true;
}

void GridClass::ShutdownBuffers()
{
    // 인덱스 버퍼를 해제합니다.
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    // 정점 버퍼를 해제합니다.
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }
}

void GridClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    // 정점 버퍼의 단위와 오프셋을 설정합니다.
    UINT stride = sizeof(VertexType);
    UINT offset = 0;

    // 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정합니다.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool GridClass::LoadTexture(ID3D11Device* device, HWND hwnd, WCHAR* Filename1)
{
    // 텍스처 오브젝트를 생성한다.
    m_Texture = new TextureClass;
    if (!m_Texture)
    {
        return false;
    }

    // 텍스처 오브젝트를 초기화한다.
    m_Texture->Initialize(device, Filename1);
}

void GridClass::ReleaseTexture()
{
    m_Texture->Shutdown();
}

bool GridClass::LoadModel(char* filename)
{
    // 모델 파일을 엽니다.
    std::ifstream fin;
    fin.open(filename);

    // 파일을 열 수 없으면 종료합니다.
    if (fin.fail())
    {
        return false;
    }

    // 버텍스 카운트의 값까지 읽는다.
    char input = 0;
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    int Vertex = 0;
    fin >> Vertex;
    // 버텍스 카운트를 읽는다.
    m_VC = Vertex;

    // 인덱스의 수를 정점 수와 같게 설정합니다.
    m_IC = m_VC;

    // 읽어 들인 정점 개수를 사용하여 모델을 만듭니다.
    m_model = new ModelType[m_VC];
    if (!m_model)
    {
        return false;
    }

    // 데이터의 시작 부분까지 읽는다.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    // 버텍스 데이터를 읽습니다.
    for (int i = 0; i < m_VC; i++)
    {
        fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
        fin >> m_model[i].tu >> m_model[i].tv;
        fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
    }

    // 모델 파일을 닫는다.
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
