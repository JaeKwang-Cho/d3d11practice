Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    float4 vLightDir[2];
    float4 vLightColor[2];
    float4 vOutputColor;
}

cbuffer cbNeverChanges : register(b1)
{
    matrix View;
};

cbuffer cbChangeOnResize : register(b2)
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b3)
{
    matrix World;
    float4 vMeshColor;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 Norm : NORMAL;
    float2 Tex : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = input.Tex;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    
    float4 finalColor = 0;
    
    // Sample : ���̴� �����Լ�, �Ķ���Ϳ� ���� ������ �޶�����.
    // ���⼭�� ���÷��� ��ǥ�� ������ �ؼ� �ؽ��ĸ� �׸��� �ȴ�.
    finalColor += txDiffuse.Sample(samLinear, input.Tex) * vMeshColor;

	//do NdotL lighting for 2 lights
    for (int i = 0; i < 2; i++)
    {
        finalColor *= saturate(dot((float3) vLightDir[i], input.Norm) * vLightColor[i]);
    }
    finalColor.a = 1;
    
    return finalColor;
}

float4 PSSolid(PS_INPUT input) : SV_Target
{
    return vOutputColor;
}

