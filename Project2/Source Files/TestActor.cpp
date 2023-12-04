#include "pch.h"
#include "TestActor.h"
#include "CMesh.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

void TestActor::StartObject()
{

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
{
	SetObjectName("TestActor");
	m_mesh = new CMesh;
	m_mesh->Initialize("obj\\eyeball.obj");
	//m_mesh->Initialize("fbx\\dodge_challenger.fbx");
	m_mesh->SetObjectName("EyeballMesh");
	AddComponent(m_mesh);
}

TestActor::~TestActor()
{
}
