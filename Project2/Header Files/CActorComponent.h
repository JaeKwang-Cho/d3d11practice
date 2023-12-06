#pragma once
#include "CObject.h"

class CActorComponent :
    public CObject
{
public:

public:
	virtual void StartObject() override;

	virtual void UpdateObject() override;

	virtual CActorComponent* Clone()
	{
		return nullptr;
	}
		
public:
	CActorComponent();
	virtual ~CActorComponent();

};

