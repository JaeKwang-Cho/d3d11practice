#include "pch.h"
#include "CScene.h"
#include "CObject.h"

void CScene::Update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			m_vecObjectGroup[i][j]->UpdateObject();
		}
	}
}

void CScene::Render()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

	// 뎁스 버퍼 지우기
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

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

CScene::CScene()
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			m_vecObjectGroup[i][j]->Render();
		}
	}
}
