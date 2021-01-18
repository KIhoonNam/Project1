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


    // 첫 번째 텍스처에서 픽셀 색상을 가져옵니다.
    color1 = shaderTextures[0].Sample(SampleType, input.tex);

    // 두 번째 텍스처에서 픽셀 색상을 가져옵니다.
    color2 = shaderTextures[1].Sample(SampleType, input.tex);

    // 두 픽셀을 혼합하고 감마 값을 곱합니다.
    blendColor = color1 * color2 * 1.5;

    // 최종 색상을 채 웁니다.
    blendColor = saturate(blendColor);

    return blendColor;
}
