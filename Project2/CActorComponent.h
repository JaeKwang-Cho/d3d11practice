#pragma once
#include "CObject.h"

class CActorComponent :
    public CObject
{
public:
	virtual void AddComponent(const CObject* _comp) override;
	virtual void AddSubObject(const CObject* _subObj) override;

public:
	virtual void Start() override;

	virtual void UpdateObject() override;
	virtual void UpdateComponent() override;
	virtual void Render() override;

	virtual CActorComponent* Clone()
	{
		return nullptr;
	}
		
public:
	CActorComponent();
	virtual ~CActorComponent();

};

