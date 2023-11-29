#include "pch.h"
#include "CScene.h"
#include "CObject.h"

void CScene::Enter()
{
	EnterScene();

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			m_vecObjectGroup[i][j]->Start();
		}
	}

	CreateDefaultTextureSampler();
}

void CScene::Update()
{
	UpdateScene();

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			m_vecObjectGroup[i][j]->Update();
		}
	}
}

void CScene::Render()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	// ���� ���� �����
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// �⺻ ���÷� �������ֱ�
	g_pImmediateContext->PSSetSamplers(0, 1, &m_TextureSampler);

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			m_vecObjectGroup[i][j]->Render();
		}
	}

	// ���۸� ȭ�鿡 ���.
	g_pSwapChain->Present(0, 0);
}

HRESULT CScene::CreateDefaultTextureSampler()
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
	m_TextureSampler->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CScene::CreateDefaultTextureSampler") - 1, "CScene::CreateDefaultTextureSampler");
	return hr;
}

CScene::CScene()
	:m_TextureSampler(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			delete m_vecObjectGroup[i][j];
		}
	}
	if (m_TextureSampler) m_TextureSampler->Release();
}

