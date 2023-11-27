#include "pch.h"
#include "CSceneManager.h"
#include "CDefault_Scene.h"

void CSceneManager::Init()
{
	m_SceneArr[(UINT)SCENE_TYPE::Default] = new CDefault_Scene;
	m_SceneArr[(UINT)SCENE_TYPE::Default]->SetSceneName(L"Default_Scene");

	m_pCurrScene = m_SceneArr[(UINT)SCENE_TYPE::Default];
	m_pCurrScene->Enter();
}

void CSceneManager::Update()
{
	m_pCurrScene->Update();
}

void CSceneManager::Render()
{
	m_pCurrScene->Render();
}

CSceneManager::CSceneManager()
	:m_SceneArr{}
	, m_pCurrScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; i++)
	{
		if (m_SceneArr[i] != nullptr)
		{ 
			delete m_SceneArr[i];
		}
	}
}