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
    
    float3 dynamicColor;
    float dynamicStrength;
    
    float3 dynamicPosition;
    float attenuation_a;

    float attenuation_b;
    float attenuation_c;
    float2 dummy;
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
    float3 WorldPos : WORLD_POSITION;
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
    output.WorldPos = normalize(mul(input.Pos, WorldMat));

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    
    float4 SampleColor = txDiffuse.Sample(samLinear, input.Tex);
    //float4 SampleColor = float4(input.Normal, 1.f);
    
    // Sample : 쉐이더 내장함수, 파라미터에 따라 동작이 달라진다.
    // 여기서는 샘플러와 좌표로 보간을 해서 텍스쳐를 그리게 된다.
    
    // 엠비언트
    float4 ambientLight;
    {
        ambientLight = float4(ambientColor * ambientStrength, 1.f);
    }
    float4 appliedLight = ambientLight;
    
    // 디퓨즈
    float diffuseLight;
    {
        float3 vectorToLight = normalize(dynamicPosition - input.WorldPos);
    
        float diffuseLightIntensity = max(dot(vectorToLight, input.Normal), 0);
    
        diffuseLight = diffuseLightIntensity * dynamicStrength * dynamicColor;
    }
    
    float distanceToLight = distance(dynamicPosition, input.WorldPos);
    
    // 세기 감쇠
    float attenuationFactor;
    {
        attenuationFactor = 
            1 / (attenuation_a + 
            attenuation_b * distanceToLight + 
            attenuation_c * pow(distanceToLight, 2));
    }
    diffuseLight *= saturate(attenuationFactor);
    
    appliedLight += diffuseLight;
    
    float4 finalColor = SampleColor * saturate(appliedLight);
    
    finalColor.a = input.Alpha;
    
    return finalColor;
}
