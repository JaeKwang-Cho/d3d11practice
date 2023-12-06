Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbMVPMatrix : register(b0)
{
    matrix RenderMat;
    matrix WorldMat;
};

cbuffer lightBuffer : register(b1)
{
    float3 ambientColor;
    float ambientStrength;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
    float Alpha : ALPHA;
    float3 Normal : NORMAL;
    //float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
    float Alpha : ALPHA;
    float3 Normal : NORMAL;
    //float4 Color : COLOR;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = mul(input.Pos, RenderMat);
    output.Alpha = input.Alpha;
    //output.Color = input.Color;
    //output.Norm = mul(input.Normal, World);
    
    output.Tex = input.Tex;
    output.Normal = normalize(mul(float4(input.Normal, 0.f), WorldMat));

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    
    //float4 SampleColor = txDiffuse.Sample(samLinear, input.Tex);
    float4 SampleColor = float4(input.Normal, 1.f);
    
    // Sample : 쉐이더 내장함수, 파라미터에 따라 동작이 달라진다.
    // 여기서는 샘플러와 좌표로 보간을 해서 텍스쳐를 그리게 된다.
    SampleColor.a = input.Alpha;
    
    float4 ambientLight = float4(ambientColor, 1.f) * ambientStrength;
    float4 finalColor = SampleColor * ambientLight;
    
    return finalColor;
}
