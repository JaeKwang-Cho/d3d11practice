#pragma once

class CObject;
class CLight;

class CScene
{
private:
	vector<CObject*> m_vecObjectGroup[(UINT)GROUP_TYPE::END];
	wstring m_Name;

protected:
	CameraStruct m_CameraStruct;

protected:
	CLight*	m_pSceneLight;

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType);

public:
	void SetSceneName(const wstring& _Name){m_Name = _Name;}
	const wstring& GetSceneName(){return m_Name;}
	CLight* GetSceneLight(){return m_pSceneLight;}
	CameraStruct GetCameraStruct(){return m_CameraStruct;}

public:
	void Enter();
	void Update();
	void Render();

	virtual void EnterScene() = 0;
	virtual void UpdateScene() = 0;
	virtual void Exit() = 0;

private:
	void InitCamera();
	void FlyCamera();

public:
	CScene();
	virtual ~CScene();
};

