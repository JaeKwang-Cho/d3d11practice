#include "pch.h"
#include "CDefault_Scene.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CMouseManager.h"

#include "TestActor.h"
#include "TestLight.h"

void CDefault_Scene::EnterScene()
{
    // ����
	TestActor* tempActor = new TestActor;
    // ����
    TestLight* tempLight = new TestLight;

	AddObject(tempActor, GROUP_TYPE::Default);
    AddObject(tempLight, GROUP_TYPE::Default);

    m_SceneLight.ambientColor = FLOAT3(1.f, 1.f, 1.f);
    m_SceneLight.ambientStrength = 1.f;
}

void CDefault_Scene::UpdateScene()
{
    if (GetFocus() != CCore::GetInstance()->GetMainHwnd())
    {
        return;
    }

    if (KEYINPUTTAP(KEY::RMOUSE))
    {
        ShowCursor(false);
    }

    if (KEYINPUTAWAY(KEY::RMOUSE) || KEYINPUTCHECK(KEY::RMOUSE, KEY_STATE::KS_NONE))
    {
        ShowCursor(true);
    }

    if (KEYINPUTAWAY(KEY::LMOUSE))
    {
        ID3D11Resource* pResource = nullptr;
        g_pRenderTargetView->GetResource(&pResource);
        GetShaderResourceFromViewToFile(pResource);
        if (pResource) pResource->Release();
    }
}

void CDefault_Scene::Exit()
{
    
}

CDefault_Scene::CDefault_Scene()
{
}

CDefault_Scene::~CDefault_Scene()
{
}
