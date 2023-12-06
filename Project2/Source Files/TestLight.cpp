#include "pch.h"
#include "TestLight.h"
#include "CMesh.h"
#include "CKeyManager.h"

void TestLight::UpdateObject()
{
	if (KEYINPUTTAP(KEY::C))
	{
		FLOAT3 CameraPos = FLOAT3(GetOwnerScene()->GetCameraStruct().At.x, GetOwnerScene()->GetCameraStruct().At.y, GetOwnerScene()->GetCameraStruct().At.z);
		SetWorldPosition(CameraPos);
	}
}

TestLight::TestLight()
	: CLight()
	, m_lightmesh(nullptr)
{
	SetObjectName("Testlight");
	m_lightmesh = new CMesh;
	m_lightmesh->Initialize("fbx\\light.fbx");
	//m_mesh->Initialize("fbx\\dodge_challenger.fbx");
	m_lightmesh->SetObjectName("lightmesh");
	AddComponent(m_lightmesh);
}

TestLight::~TestLight()
{
}
