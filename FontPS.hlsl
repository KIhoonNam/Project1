////////////////////////////////////////////////////////////////////////////////
// Filename: font.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PixelBuffer
{
    float4 pixelColor;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
    float4 color;


// �� ��ġ���� �ؽ�ó �ȼ��� ���ø��մϴ�.
color = shaderTexture.Sample(SampleType, input.tex);

// �ؽ�ó�� ������ ���� ���̸��� �ȼ��� �������� ó���մϴ�.
if (color.r == 0.0f)
{
    color.a = 0.0f;
}

// �ؽ�ó�� ���� ���� �ƴ� ��� �۲��� �ȼ��̹Ƿ� �۲� �ȼ� ������ ����Ͽ� �׸��ϴ�.
else
{
    color.rgb = pixelColor.rgb;
    color.a = 1.0f;
}

return color;
}