#pragma once

class CObject;
class CLight;

class CScene
{
private:
	vector<CObject*> m_vecObjectGroup[(UINT)GROUP_TYPE::END];
	wstring m_Name;

protected:
	CameraStruct m_CameraStruct3D;
	CameraStruct m_CameraStruct2D;

protected:
	CLight*	m_pSceneLight;

public:
	void AddObject(CObject* _pObj, GROUP_TYPE _eType);

public:
	void SetSceneName(const wstring& _Name){m_Name = _Name;}
	const wstring& GetSceneName(){return m_Name;}
	CLight* GetSceneLight(){return m_pSceneLight;}
	CameraStruct GetCameraStruct3D(){return m_CameraStruct3D;}
	CameraStruct GetCameraStruct2D(){return m_CameraStruct2D;}

public:
	void Enter();
	void Update();

	virtual void EnterScene() = 0;
	virtual void UpdateScene() = 0;
	virtual void Exit() = 0;

private:
	void InitCamera3D();
	void InitCamera2D();
	void FlyCamera();

public:
	CScene();
	virtual ~CScene();
};

