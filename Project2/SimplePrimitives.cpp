#include "pch.h"
#include "SimplePrimitives.h"

DefaultVertex DefaultCube[] =
{
	{ FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f),FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f),FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f), FLOAT2(0.0f, 1.0f) },
};

WORD indices[] =
{
	3,1,0,
	2,1,3,

	6,4,5,
	7,4,6,

	11,9,8,
	10,9,11,

	14,12,13,
	15,12,14,

	19,17,16,
	18,17,19,

	22,20,21,
	23,20,22
};

void GetSimpleCube(_Outptr_ DefaultVertex** _CubeVetices, _Outptr_ PWORD* _CubeIndicies)
{
	*_CubeVetices = DefaultCube;
	*_CubeIndicies = indices;
}

