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


CScene::CScene()
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
}

