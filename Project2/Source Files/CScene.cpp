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

	// 뎁스 버퍼 지우기
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// 기본 샘플러 설정해주기
	g_pImmediateContext->PSSetSamplers(0, 1, &m_TextureSampler);

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			m_vecObjectGroup[i][j]->Render();
		}
	}

	// 버퍼를 화면에 쏜다.
	g_pSwapChain->Present(0, 0);
}

HRESULT CScene::CreateDefaultTextureSampler()
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

