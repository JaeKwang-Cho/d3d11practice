#include "pch.h"
#include "CRenderManager.h"
#include "MeshComp.h"

void CRenderManager::Init()
{
    CreateConstantBuffer();

    CreateDefaultTextureSampler();
}

void CRenderManager::Update()
{
}

void CRenderManager::Render()
{
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // 뎁스 버퍼 지우기
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // 기본 샘플러 설정해주기
    g_pImmediateContext->PSSetSamplers(0, 1, &m_TextureSampler);

    // 계산을 돌리고 인덱스를 따라 삼각형을 그리도록 시킨다.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 불투명한 애들을 출력해준다.
    auto iterOut = m_NonAlphaMeshes.begin();
    for (; iterOut != m_NonAlphaMeshes.end(); iterOut++)
    {
        auto iterIn = (*iterOut)->begin();
        for (; iterIn != (*iterOut)->end(); iterIn++)
        {
            (*iterIn)->RenderComp();
        }
    }


    // 반 투명한 애들을 출력해준다.
    while (!m_AlphaMeshes.empty())
    {
        MeshComp* pMeshComp = m_AlphaMeshes.top();
        pMeshComp->RenderComp();
        m_AlphaMeshes.pop();
    }

    // 버퍼를 화면에 쏜다.
    g_pSwapChain->Present(0, 0);

    m_NonAlphaMeshes.clear();
}

void CRenderManager::EnqueAlphaMeshComp(MeshComp* _alphaMesh)
{
    m_AlphaMeshes.push(_alphaMesh);
}

void CRenderManager::EnqueNonAlphaMeshComps(vector<MeshComp*>* _nonAlphaMeshes)
{
    m_NonAlphaMeshes.push_back(_nonAlphaMeshes);
}

HRESULT CRenderManager::CreateDefaultTextureSampler()
{
    // 텍스쳐의 Sampler state 만들기
    D3D11_SAMPLER_DESC sampDesc;
    memset(&sampDesc, 0, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 조합해서 쓸 수 있다. 지금은 선형 보간하는데 쓰인다. 
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap은 값이 넘어간다면 0 ~ 1 , 1 ~ 0 으로 반복이 되는 것이고
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Mirror는 0 ~ 1 ~ 0으로 반복이 된다.
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // Clamp는 0 ~ 1을 벗어나면, 마지막 픽셀로 채워진다.
    // Border는 넘어가면 Border Color로 지정해준 색으로 채워진다.
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; // 셈플링된 데이터를 기존 샘플링된 데이터와 비교하는 함수
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;


    HRESULT hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &m_TextureSampler);
    m_TextureSampler->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CRenderManager::CreateDefaultTextureSampler") - 1, "CRenderManager::CreateDefaultTextureSampler");
    return hr;
}

HRESULT CRenderManager::CreateConstantBuffer()
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
        return hr;
    }
    g_pCBMVPMat->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CRenderManager::Init - g_pCBMVPMat") - 1, "CDefault_Scene::EnterScene - g_pCBNeverChanges");

    return hr;
}

CRenderManager::CRenderManager()
    :m_TextureSampler(nullptr)
{

}

CRenderManager::~CRenderManager()
{
    if (g_pCBMVPMat) g_pCBMVPMat->Release();
    if (m_TextureSampler) m_TextureSampler->Release();
}
