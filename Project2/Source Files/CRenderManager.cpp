#include "pch.h"
#include "CRenderManager.h"
#include "MeshComp.h"

void CRenderManager::Init()
{
    // Constant 버퍼 만들기
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.CPUAccessFlags = 0;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    bd.ByteWidth = sizeof(MVPMatrix);
    HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBMVPMat);
    if (FAILED(hr))
    {
        assert(false && "CreateBuffer Failed");
    }
    g_pCBMVPMat->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CRenderManager::Init - g_pCBMVPMat") - 1, "CDefault_Scene::EnterScene - g_pCBNeverChanges");
}

void CRenderManager::Update()
{

}

void CRenderManager::Render()
{
}

CRenderManager::CRenderManager()
{

}

CRenderManager::~CRenderManager()
{
    if (g_pCBMVPMat) g_pCBMVPMat->Release();
}