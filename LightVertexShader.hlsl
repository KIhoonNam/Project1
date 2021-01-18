////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output = (PixelInputType)0;


    // ������ ��� ����� ���� ��ġ ���͸� 4 ������ �����մϴ�.
    input.position.w = 1.0f;

    // ����, �� �� ���� ��Ŀ� ���� ������ ��ġ�� ??����մϴ�.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // �ȼ� ���̴��� �ؽ�ó ��ǥ�� �����Ѵ�.
    output.tex = input.tex;

    // ���� ��Ŀ� ���ؼ��� ���� ���͸� ����մϴ�.
    output.normal = mul(input.normal, (float3x3)worldMatrix);

    // ���� ���͸� ����ȭ�մϴ�.
    output.normal = normalize(output.normal);

    // ������ ���� ��ġ�� ����մϴ�.
    float4 worldPosition = mul(input.position, worldMatrix);

    // ī�޶��� ��ġ�� ������ ���� ��ġ�� �������� ���� ������ �����մϴ�.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

    // �� ���� ���͸� ǥ��ȭ�մϴ�.
    output.viewDirection = normalize(output.viewDirection);

    return output;
}