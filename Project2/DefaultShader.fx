Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

/*
cbuffer ConstantBuffer : register(b0)
{
    float4 vLightDir[2];
    float4 vLightColor[2];
}
*/

cbuffer cbMVPMatrix : register(b0)
{
    matrix Mat;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
    float Alpha : ALPHA;
    //float3 Normal : NORMAL;
    //float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
    float Alpha : ALPHA;
     
    //float3 Norm : NORMAL;
    //float4 Color : COLOR;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = mul(input.Pos, Mat);
    output.Alpha = input.Alpha;
    //output.Color = input.Color;
    //output.Norm = mul(input.Normal, World);
    
    output.Tex = input.Tex;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    
    float4 finalColor = 0;
    
    // Sample : 쉐이더 내장함수, 파라미터에 따라 동작이 달라진다.
    // 여기서는 샘플러와 좌표로 보간을 해서 텍스쳐를 그리게 된다.
    finalColor += txDiffuse.Sample(samLinear, input.Tex);
    finalColor.a = input.Alpha;
	//do NdotL lighting for 2 lights
    /*
    for (int i = 0; i < 2; i++)
    {
        finalColor *= saturate(dot((float3) vLightDir[i], input.Norm) * vLightColor[i]);
    }
    finalColor.a = 1;
    */
    //finalColor = input.Color;
    

    return finalColor;
}
