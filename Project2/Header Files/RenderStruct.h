#pragma once
#include "struct.h"

extern D3D11_INPUT_ELEMENT_DESC g_DefaultLayout[];
extern UINT g_DefaultLayoutNumElements;

struct CameraStruct {
    Vector4 Eye;
    Vector4 Direction;
    Vector4 At;
    Vector4 Up;
    Vector4 CameraLeft;
    Vector4 CameraUp;
};

struct SampleVertex {
    FLOAT3 Pos;
    FLOAT4 Color;

    FLOAT3 Normal;
    FLOAT2 TEX;
};

struct DefaultVertex {
    FLOAT3 Pos;
    FLOAT2 Tex;
    float Alpha;
    FLOAT3 Normal;
    //FLOAT4 Color;

    DefaultVertex()
        :Alpha(1.f)
    {
    }
    DefaultVertex(float x, float y, float z, float u, float v, float a, float nx, float ny, float nz)
        :Pos(x, y, z), Tex(u, v), Alpha(a), Normal(nx, ny, nz)
    {
    }

    DefaultVertex(FLOAT3 _Pos, FLOAT2 _Tex, float _Alpha, FLOAT3 _Norm)
        :Pos(_Pos), Tex(_Tex), Alpha(_Alpha), Normal(_Norm)
    {
    }
};

struct MVPMatrix
{
    Matrix RenderMat;
    Matrix WorldMat;
};

struct LightBuffer
{
    FLOAT3 ambientColor;
    float ambientStrength;
};