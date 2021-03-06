#include "stdafx.h"
#include "FontClass.h"
#include "FontShaderClass.h"
#include "TextClass.h"


TextClass::TextClass()
{
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth,
    int screenHeight, XMMATRIX baseViewMatrix)
{
    bool result;


    // Store the screen width and height.
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Store the base view matrix.
    m_baseViewMatrix = baseViewMatrix;

    // Create the font object.
    m_Font = new FontClass;
    if (!m_Font)
    {
        return false;
    }

    char ch[] = "../Project1/data/fontdata.txt";
    WCHAR wch[] = L"../Project1/data/font.dds";

    // Initialize the font object.
    result = m_Font->Initialize(device,ch , wch);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
        return false;
    }

    // Create the font shader object.
    m_FontShader = new FontShaderClass;
    if (!m_FontShader)
    {
        return false;
    }

    // Initialize the font shader object.
    result = m_FontShader->Initialize(device, hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
        return false;
    }

    // Initialize the first sentence.
    result = InitializeSentence(&m_sentence1, 16, device);
    if (!result)
    {
        return false;
    }

    char Hi[] = "Fps";

    // Now update the sentence vertex buffer with the new string information.
    result = UpdateSentence(m_sentence1,Hi, 500, 300, 1.0f, 1.0f, 1.0f, deviceContext);
    if (!result)
    {
        return false;
    }

    // Initialize the first sentence.
    result = InitializeSentence(&m_sentence2, 16, device);
    if (!result)
    {
        return false;
    }
    char Goodbye[] = "Cpu";
    // Now update the sentence vertex buffer with the new string information.
    result = UpdateSentence(m_sentence2,Goodbye , 300, 200, 1.0f, 1.0f, 0.0f, deviceContext);
    if (!result)
    {
        return false;
    }

    return true;
}


void TextClass::Shutdown()
{
    // Release the first sentence.
    ReleaseSentence(&m_sentence1);

    // Release the second sentence.
    ReleaseSentence(&m_sentence2);

    // Release the font shader object.
    if (m_FontShader)
    {
        m_FontShader->Shutdown();
        delete m_FontShader;
        m_FontShader = 0;
    }

    // Release the font object.
    if (m_Font)
    {
        m_Font->Shutdown();
        delete m_Font;
        m_Font = 0;
    }

    return;
}


bool TextClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix)
{
    bool result;


    // Draw the first sentence.
    result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
    if (!result)
    {
        return false;
    }

    // Draw the second sentence.
    result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
    if (!result)
    {
        return false;
    }

    return true;
}


bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;


    // Create a new sentence object.
    *sentence = new SentenceType;
    if (!*sentence)
    {
        return false;
    }

    // Initialize the sentence buffers to null.
    (*sentence)->vertexBuffer = 0;
    (*sentence)->indexBuffer = 0;

    // Set the maximum length of the sentence.
    (*sentence)->maxLength = maxLength;

    // Set the number of vertices in the vertex array.
    (*sentence)->vertexCount = 6 * maxLength;

    // Set the number of indexes in the index array.
    (*sentence)->indexCount = (*sentence)->vertexCount;

    // Create the vertex array.
    vertices = new VertexType[(*sentence)->vertexCount];
    if (!vertices)
    {
        return false;
    }

    // Create the index array.
    indices = new unsigned long[(*sentence)->indexCount];
    if (!indices)
    {
        return false;
    }

    // Initialize vertex array to zeros at first.
    memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

    // Initialize the index array.
    for (i = 0; i < (*sentence)->indexCount; i++)
    {
        indices[i] = i;
    }

    // Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Release the vertex array as it is no longer needed.
    delete[] vertices;
    vertices = 0;

    // Release the index array as it is no longer needed.
    delete[] indices;
    indices = 0;

    return true;
}


bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green,
    float blue, ID3D11DeviceContext* deviceContext)
{
    int numLetters;
    VertexType* vertices;
    float drawX, drawY;
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    VertexType* verticesPtr;


    // Store the color of the sentence.
    sentence->red = red;
    sentence->green = green;
    sentence->blue = blue;

    // Get the number of letters in the sentence.
    numLetters = (int)strlen(text);

    // Check for possible buffer overflow.
    if (numLetters > sentence->maxLength)
    {
        return false;
    }

    // Create the vertex array.
    vertices = new VertexType[sentence->vertexCount];
    if (!vertices)
    {
        return false;
    }

    // Initialize vertex array to zeros at first.
    memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

    // Calculate the X and Y pixel position on the screen to start drawing to.
    drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
    drawY = (float)((m_screenHeight / 2) - positionY);

    // Use the font class to build the vertex array from the sentence text and sentence draw location.
    m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

    // Lock the vertex buffer so it can be written to.
    result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // Get a pointer to the data in the vertex buffer.
    verticesPtr = (VertexType*)mappedResource.pData;

    // Copy the data into the vertex buffer.
    memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

    // Unlock the vertex buffer.
    deviceContext->Unmap(sentence->vertexBuffer, 0);

    // Release the vertex array as it is no longer needed.
    delete[] vertices;
    vertices = 0;

    return true;
}


void TextClass::ReleaseSentence(SentenceType** sentence)
{
    if (*sentence)
    {
        // Release the sentence vertex buffer.
        if ((*sentence)->vertexBuffer)
        {
            (*sentence)->vertexBuffer->Release();
            (*sentence)->vertexBuffer = 0;
        }

        // Release the sentence index buffer.
        if ((*sentence)->indexBuffer)
        {
            (*sentence)->indexBuffer->Release();
            (*sentence)->indexBuffer = 0;
        }

        // Release the sentence.
        delete* sentence;
        *sentence = 0;
    }

    return;
}


bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX worldMatrix,
    XMMATRIX orthoMatrix)
{
    unsigned int stride, offset;
    XMFLOAT4 pixelColor;
    bool result;


    // Set vertex buffer stride and offset.
    stride = sizeof(VertexType);
    offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Create a pixel color vector with the input sentence color.
    pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

    // Render the text using the font shader.
    result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix,
        m_Font->GetTexture(), pixelColor);
    if (!result)
    {
        false;
    }

    return true;
}
bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
    // fps를 10,000 이하로 자릅니다.
    if (fps > 9999)
    {
        fps = 9999;
    }

    // fps 정수를 문자열 형식으로 변환합니다.
    char tempString[16] = { 0, };
    _itoa_s(fps, tempString, 10);

    // fps 문자열을 설정합니다.
    char fpsString[16] = { 0, };
    strcpy_s(fpsString, "Fps: ");
    strcat_s(fpsString, tempString);

    float red = 0;
    float green = 0;
    float blue = 0;

    // fps가 60 이상이면 fps 색상을 녹색으로 설정합니다.
    if (fps >= 60)
    {
        red = 0.0f;
        green = 1.0f;
        blue = 0.0f;
    }

    // fps가 60보다 작은 경우 fps 색상을 노란색으로 설정합니다.
    if (fps < 60)
    {
        red = 1.0f;
        green = 1.0f;
        blue = 0.0f;
    }

    // fps가 30 미만이면 fps 색상을 빨간색으로 설정합니다.
    if (fps < 30)
    {
        red = 1.0f;
        green = 0.0f;
        blue = 0.0f;
    }

    // 문장 정점 버퍼를 새 문자열 정보로 업데이트합니다.
    return UpdateSentence(m_sentence1, fpsString, 600, 20, red, green, blue, deviceContext);
}


bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
    // cpu 정수를 문자열 형식으로 변환합니다.
    char tempString[16] = { 0, };
    _itoa_s(cpu, tempString, 10);

    // cpu 문자열을 설정합니다.
    char cpuString[16] = { 0, };
    strcpy_s(cpuString, "Cpu: ");
    strcat_s(cpuString, tempString);
    strcat_s(cpuString, "%");

    // 문장 정점 버퍼를 새 문자열 정보로 업데이트합니다.
    return UpdateSentence(m_sentence2, cpuString, 600, 40, 0.0f, 1.0f, 0.0f, deviceContext);
}
bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
    // mouseX 정수를 문자열 형식으로 변환합니다.
    char tempString[16] = { 0, };
    _itoa_s(mouseX, tempString, 10);

    // mouseX 문자열을 설정합니다.
    char mouseString[16] = { 0, };
    strcpy_s(mouseString, "Mouse X: ");
    strcat_s(mouseString, tempString);

    // 문장 정점 버퍼를 새 문자열 정보로 업데이트합니다.
    if (!UpdateSentence(m_sentence1, mouseString, 600, 20, 1.0f, 1.0f, 1.0f, deviceContext))
    {
        return false;
    }

    // mouseY 정수를 문자열 형식으로 변환합니다.
    _itoa_s(mouseY, tempString, 10);

    // mouseY 문자열을 설정합니다.
    strcpy_s(mouseString, "Mouse Y: ");
    strcat_s(mouseString, tempString);

    // 문장 정점 버퍼를 새 문자열 정보로 업데이트합니다.
    if (!UpdateSentence(m_sentence2, mouseString, 600, 40, 1.0f, 1.0f, 1.0f, deviceContext))
    {
        return false;
    }

    return true;
}