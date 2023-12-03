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

    // ���� ���� �����
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // �⺻ ���÷� �������ֱ�
    g_pImmediateContext->PSSetSamplers(0, 1, &m_TextureSampler);

    // ����� ������ �ε����� ���� �ﰢ���� �׸����� ��Ų��.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // �������� �ֵ��� ������ش�.
    auto iterOut = m_NonAlphaMeshes.begin();
    for (; iterOut != m_NonAlphaMeshes.end(); iterOut++)
    {
        auto iterIn = (*iterOut)->begin();
        for (; iterIn != (*iterOut)->end(); iterIn++)
        {
            (*iterIn)->RenderComp();
        }
    }


    // �� ������ �ֵ��� ������ش�.
    while (!m_AlphaMeshes.empty())
    {
        MeshComp* pMeshComp = m_AlphaMeshes.top();
        pMeshComp->RenderComp();
        m_AlphaMeshes.pop();
    }

    // ���۸� ȭ�鿡 ���.
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
    // �ؽ����� Sampler state �����
    D3D11_SAMPLER_DESC sampDesc;
    memset(&sampDesc, 0, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // �����ؼ� �� �� �ִ�. ������ ���� �����ϴµ� ���δ�. 
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap�� ���� �Ѿ�ٸ� 0 ~ 1 , 1 ~ 0 ���� �ݺ��� �Ǵ� ���̰�
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Mirror�� 0 ~ 1 ~ 0���� �ݺ��� �ȴ�.
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // Clamp�� 0 ~ 1�� �����, ������ �ȼ��� ä������.
    // Border�� �Ѿ�� Border Color�� �������� ������ ä������.
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; // ���ø��� �����͸� ���� ���ø��� �����Ϳ� ���ϴ� �Լ�
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;


    HRESULT hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &m_TextureSampler);
    m_TextureSampler->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CRenderManager::CreateDefaultTextureSampler") - 1, "CRenderManager::CreateDefaultTextureSampler");
    return hr;
}

HRESULT CRenderManager::CreateConstantBuffer()
{
    // Constant ���� �����
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
