#pragma once

class CObject;

class CScene
{
	vector<CObject*> m_vecObjectGroup[(UINT)GROUP_TYPE::END];
	wstring m_Name;

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType)
	{
		m_vecObjectGroup[(UINT)_eType].push_back(_pObj);
	}

public:
	void SetSceneName(const wstring& _Name){m_Name = _Name;}
	const wstring& GetSceneName(){return m_Name;}

	void Update();
	void Render();

	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:
	CScene();
	virtual ~CScene();
};

