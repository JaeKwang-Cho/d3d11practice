#pragma once

class CScene;

class CSceneManager
{
	SINGLETON(CSceneManager);

private:
	// Ÿ�� ���� �ϳ��� Scene�� ������ �ִ´�.
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
