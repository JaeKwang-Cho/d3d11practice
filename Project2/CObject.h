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
	vector<const CObject*> m_Components;
public:
	void SetOwnerObject(CObject* const _OwnerObject) {
		m_OwnerObject = _OwnerObject;
	 }
	virtual void AddComponent(const CObject* _comp);
	virtual void AddSubObject(const CObject* _subObj);
public:
	virtual void Start() = 0;

	virtual void UpdateObject() = 0;
	virtual void UpdateComponent();
	virtual void Render() = 0;

	virtual CObject* Clone() = 0;

	void Render_Component();

public:
	CObject();
	CObject(const CObject& _other);
	virtual ~CObject();

};

