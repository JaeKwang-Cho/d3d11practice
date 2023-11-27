#include "pch.h"
#include "TestActor.h"
#include "CMesh.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

void TestActor::StartObject()
{
	/*
	for (int i = 0; i < 1000; i++)
	{
		pRAs[i] = new CRenderAsset();
		pRAs[i]->SetOwnerObject(this);

		int x = i % 10 - 5;
		int y = (i % 100) / 10 - 5;
		int z = i / 100 - 5;

		FLOAT3 pos = FLOAT3((float)x * 10.f, (float)y * 10.f, (float)z * 10.f);
		pRAs[i]->SetRelativePosition(pos);

		AddComponent(pRAs[i]);
	}
	*/

	m_mesh = new CMesh;
	m_mesh->Initialize("fbx\\eyeball.fbx");
	AddComponent(m_mesh);
}

void TestActor::UpdateObject()
{
	if (KEYINPUTTAP(KEY::RIGHT))
	{
		Transform transform = GetWorldTransform();
		transform.Position += FLOAT3(1.f , 0.f, 0.f);
		SetWorldPosition(transform.Position);
	}
	if (KEYINPUTTAP(KEY::UP))
	{
		Transform transform = GetWorldTransform();
		transform.Position += FLOAT3(0.f, 0.f, 1.f);
		SetWorldPosition(transform.Position);
	}
	if (KEYINPUTTAP(KEY::LEFT))
	{
		Transform transform = GetWorldTransform();
		transform.Position += FLOAT3(-1.f, 0.f, 0.f);
		SetWorldPosition(transform.Position);
	}
	if (KEYINPUTTAP(KEY::DOWN))
	{
		Transform transform = GetWorldTransform();
		transform.Position += FLOAT3(0.f, 0.f, -1.f);
		SetWorldPosition(transform.Position);
	}

	if (KEYINPUTTAP(KEY::R))
	{
		Transform transform = GetWorldTransform();
		transform.Rotation += FLOAT3(0.f, 1.f, 0.f);
		SetWorldRotation(transform.Rotation);
	}

	if (KEYINPUTTAP(KEY::B))
	{
		Transform transform = GetWorldTransform();
		transform.Scale += FLOAT3(0.2f, 0.2f, 0.2f);
		SetWorldScale(transform.Scale);
	}

	if (KEYINPUTTAP(KEY::N))
	{
		Transform transform = GetWorldTransform();
		transform.Scale += FLOAT3(-0.2f, -0.2f, -0.2f);
		SetWorldScale(transform.Scale);
	}
}

TestActor::TestActor()
	:m_mesh(nullptr)
	//,pRAs{}
{
}

TestActor::~TestActor()
{
}
