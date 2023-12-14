#pragma once
#include "CScene.h"

class CObject
{
protected:
	// Property
	string		m_Name;
	bool		m_bAlive;
	bool		m_bRenderComponent;

	// Owner
	CObject* m_OwnerObject;
	CScene* m_OwnerScene;
	Transform m_RelativeTransform;
	Transform m_WorldTransform;

	// Components or SubObject;
	vector<CObject*> m_Components;
public:
	void SetOwnerObject(CObject* const _OwnerObject) {
		m_OwnerObject = _OwnerObject;
	}
	void SetOwnerScene(CScene* const _OwnerScene)
	{
		m_OwnerScene = _OwnerScene;
		auto iter = m_Components.begin();
		for (; iter != m_Components.end(); iter++)
		{
			(*iter)->SetOwnerScene(_OwnerScene);
		}
	}
	CScene* GetOwnerScene()
	{
		return m_OwnerScene;
	}

	void AddComponent(CObject* _comp)
	{
		m_Components.push_back(_comp);
		_comp->m_OwnerObject = this;
	}

	void SetObjectName(string _Name)
	{
		m_Name = _Name;
	}

	string GetObjectName()
	{
		return m_Name;
	}

public:
	void Start();
	virtual void StartObject() = 0;
	void StartComponent();
	
	void Update();
	virtual void UpdateObject() = 0;
	void UpdateComponent();

	virtual CObject* Clone() = 0;
protected:
	virtual void FindWorldTransform() = 0;

public:
	CObject();
	CObject(const CObject& _other);
	virtual ~CObject();


public:
	const Transform& GetWorldTransform()
	{
		return m_WorldTransform;
	}

	const Transform& GetRelativeTransform()
	{
		return m_RelativeTransform;
	}

public:
	void SetRelativePosition(float _x, float _y, float _z)
	{
		m_RelativeTransform.Position = FLOAT3(_x, _y, _z);
	}

	void SetRelativePosition(FLOAT3 _pos)
	{
		m_RelativeTransform.Position = _pos;
	}

	void SetRelativeRotation(float _xAxis, float _yAxis, float _zAxis)
	{
		m_RelativeTransform.Rotation = FLOAT3(_xAxis, _yAxis, _zAxis);
	}

	void SetRelativeRotation(FLOAT3 _Rot)
	{
		m_RelativeTransform.Rotation = _Rot;
	}

	void SetRelativeScale(float _xs, float _ys, float _zs)
	{
		m_RelativeTransform.Scale = FLOAT3(_xs, _ys, _zs);
	}

	void SetRelativeScale(FLOAT3 _Scale)
	{
		m_RelativeTransform.Scale = _Scale;
	}

};

