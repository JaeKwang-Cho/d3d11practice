#include "pch.h"
#include "TestLight.h"
#include "CMesh.h"
#include "CKeyManager.h"

void TestLight::StartObject()
{
	m_Light.dynamicPosition = GetWorldTransform().Position;
}

void TestLight::UpdateObject()
{
	if (KEYINPUTTAP(KEY::C))
	{
		CameraStruct cam = GetOwnerScene()->GetCameraStruct();
		FLOAT3 CameraPos = FLOAT3(cam.At.x, cam.At.y, cam.At.z);
		FLOAT3 CameraRot = FLOAT3(1.0f, 0.f, 0.f);
		SetWorldPosition(CameraPos);
		SetWorldRotation(CameraRot);
	}
	m_Light.dynamicPosition = GetWorldTransform().Position;
}

CObject* TestLight::Clone()
{
	return nullptr;
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

	m_Light.ambientColor = FLOAT3(1.f, 1.f, 1.f);
	m_Light.ambientStrength = 0.2f;

	m_Light.dynamicColor = FLOAT3(1.f, 1.f, 1.f);
	m_Light.dynamicStrength = 1.f;

	m_Light.attenuation_a = 1.f;
	m_Light.attenuation_b = 0.1f; 
	m_Light.attenuation_c = 0.1f;
}

TestLight::~TestLight()
{
}
