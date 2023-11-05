#pragma once
#include "CObject.h"

class CRenderAsset :
    public CObject
{
private:

public:

public:
	virtual void Start() override;

	virtual void UpdateObject() override;
	virtual void UpdateComponent() override;
	virtual void Render() override;

	virtual CRenderAsset* Clone()
	{
		return nullptr;
	};

public:
	CRenderAsset();
	virtual ~CRenderAsset();
};

