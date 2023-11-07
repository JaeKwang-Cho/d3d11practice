#include "pch.h"
#include "TestActor.h"
#include "CRenderAsset.h"

void TestActor::StartObject()
{
	CRenderAsset* pRA = new CRenderAsset();
	pRA->SetOwnerObject(this);
	AddComponent(pRA);
}

TestActor::TestActor()
{
}

TestActor::~TestActor()
{
}
