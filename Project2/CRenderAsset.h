#pragma once
#include "CObject.h"

struct ObjectRenderComp;

class CRenderAsset :
    public CObject
{
private:
	Matrix m_WorldMat;
	ObjectRenderComp* m_cube;
private:

public:

public:
	virtual void StartObject() override;

	virtual void UpdateObject() override;
	virtual void RenderObject() override;

	virtual CRenderAsset* Clone()
	{
		return nullptr;
	};

private:
	void SetWorldMat();

public:
	CRenderAsset();
	virtual ~CRenderAsset();
};

