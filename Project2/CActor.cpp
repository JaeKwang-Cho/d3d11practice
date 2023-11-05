#include "pch.h"
#include "CActor.h"

void CActor::Start()
{
}

void CActor::UpdateObject()
{
}

void CActor::UpdateComponent()
{
}

void CActor::Render()
{
}

CObject* CActor::Clone()
{
	return nullptr;
}

CActor::CActor()
	: CObject()
	, m_vPosition(FLOAT4(0.f,0.f,0.f,0.f))
	, m_vRotation(FLOAT3(0.f, 0.f, 0.f))
	, m_vScale(FLOAT3(1.f, 1.f, 1.f))
{
}

CActor::~CActor()
{
}
