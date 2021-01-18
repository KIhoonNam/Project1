////////////////////////////////////////////////////////////////////////////////
// Filename: multitexture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTextures[2];
SamplerState SampleType;


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
float4 MultiTexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 color1;
    float4 color2;
    float4 blendColor;


    // ù ��° �ؽ�ó���� �ȼ� ������ �����ɴϴ�.
    color1 = shaderTextures[0].Sample(SampleType, input.tex);

    // �� ��° �ؽ�ó���� �ȼ� ������ �����ɴϴ�.
    color2 = shaderTextures[1].Sample(SampleType, input.tex);

    // �� �ȼ��� ȥ���ϰ� ���� ���� ���մϴ�.
    blendColor = color1 * color2 * 1.5;

    // ���� ������ ä ��ϴ�.
    blendColor = saturate(blendColor);

    return blendColor;
}