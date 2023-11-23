#pragma once
#include "../struct.h"

struct SimpleVertex
{
    FLOAT3 Pos;
    FLOAT4 Color;
};

struct SimpleVertex_Norm
{
    FLOAT3 POS;
    FLOAT3 Normal;
};

struct SimpleVertex_Tex
{
    FLOAT3 Pos;
    FLOAT2 TEX;
};

struct ConstantBuffer
{
    Matrix mWorld;
    Matrix mView;
    Matrix mProjection;
};

struct ConstantBuffer_Norm
{
    Matrix mWorld;
    Matrix mView;
    Matrix mProjection;
    FLOAT4 vLightDir[2];
    FLOAT4 vLightColor[2];
    FLOAT4 vOutputColor;
};

struct CBChangeOnInput
{
    Matrix mView;
};

struct CBChangeOnResize
{
    Matrix mProjection;
};

struct CBChangesEveryFrame
{
    Matrix mWorld;
    FLOAT4 vMeshColor;
};

void                RenderCube();
void                RenderCube_Norm();
void                RenderCube_Tex();

bool                SetCube();
bool                SetCubeWithNdotL();
bool                SetCubeWithTex();