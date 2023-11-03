#pragma once
#include "CObject.h"
class CRenderAsset :
    public CObject
{
private:
	DefaultVertex* Vertices;
	PWORD* Indices;

public:
	virtual void AddComponent(const CObject* _comp) override;
	virtual void AddSubObject(const CObject* _subObj) override;
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

