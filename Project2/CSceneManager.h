#pragma once

class CScene;

class CSceneManager
{
	SINGLETON(CSceneManager);

private:
	// 타입 마다 하나씩 Scene을 가지고 있는다.
	CScene* m_SceneArr[(UINT)SCENE_TYPE::END];
	CScene* m_pCurrScene;

public:
	void Init();
	void Update();
	void Render();

public:
	CScene* GetCurrScene()
	{
		return m_pCurrScene;
	}
};
