#include "pch.h"
#include "TestActor.h"
#include "CRenderAsset.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

void TestActor::StartObject()
{
	pRA = new CRenderAsset();
	pRA->SetOwnerObject(this);
	AddComponent(pRA);
}

void TestActor::UpdateObject()
{
	if (KEYINPUTTAP(KEY::RIGHT))
	{
		Transform transform = pRA->GetRelativeTransform();
		transform.Position += FLOAT3(1.f , 0.f, 0.f);
		pRA->SetRelativePosition(transform.Position);
	}
	if (KEYINPUTTAP(KEY::UP))
	{
		Transform transform = pRA->GetRelativeTransform();
		transform.Position += FLOAT3(0.f, 0.f, 1.f);
		pRA->SetRelativePosition(transform.Position);
	}
	if (KEYINPUTTAP(KEY::LEFT))
	{
		Transform transform = pRA->GetRelativeTransform();
		transform.Position += FLOAT3(-1.f, 0.f, 0.f);
		pRA->SetRelativePosition(transform.Position);
	}
	if (KEYINPUTTAP(KEY::DOWN))
	{
		Transform transform = pRA->GetRelativeTransform();
		transform.Position += FLOAT3(0.f, 0.f, -1.f);
		pRA->SetRelativePosition(transform.Position);
	}

	if (KEYINPUTTAP(KEY::R))
	{
		Transform transform = pRA->GetRelativeTransform();
		transform.Rotation += FLOAT3(0.f, 0.f, 1.f);
		pRA->SetRelativeRotation(transform.Rotation);
	}

	if (KEYINPUTTAP(KEY::B))
	{
		Transform transform = pRA->GetRelativeTransform();
		transform.Scale += FLOAT3(0.2f, 0.2f, 0.2f);
		pRA->SetRelativeScale(transform.Scale);
	}

	if (KEYINPUTTAP(KEY::N))
	{
		Transform transform = pRA->GetRelativeTransform();
		transform.Scale += FLOAT3(-0.2f, -0.2f, -0.2f);
		pRA->SetRelativeScale(transform.Scale);
	}
}

TestActor::TestActor()
	:pRA(nullptr)
{
}

TestActor::~TestActor()
{
}
