#pragma once

class CObject
{
protected:
	// Property
	wstring		m_Name;
	bool		m_bAlive;
	bool		m_bRenderComponent;

	CObject* m_OwnerObject;
	// Components or SubObject;
	vector<CObject*> m_Components;
public:
	void SetOwnerObject(CObject* const _OwnerObject) {
		m_OwnerObject = _OwnerObject;
	 }
	void AddComponent(const CObject* _comp);
public:
	void Start();
	virtual void StartObject() = 0;
	void StartComponent();
	
	void Update();
	virtual void UpdateObject() = 0;
	void UpdateComponent();

	void Render();
	virtual void RenderObject() = 0;

	virtual CObject* Clone() = 0;

	void Render_Component();

public:
	CObject();
	CObject(const CObject& _other);
	virtual ~CObject();

};

