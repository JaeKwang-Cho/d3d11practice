#pragma once
#include "CObject.h"
class CObject2D :
    public CObject
{
private:
	Transform m_OrthoTransform;

	Matrix m_WorldMat;
	Matrix m_RenderMat;
public:
	virtual void StartObject() = 0;
	virtual void UpdateObject() = 0;
	virtual CObject* Clone() = 0;

protected:
	virtual void FindWorldTransform() override;

private:
	void UpdateWorldMat();
	void UpdateRenderMat();

public:
};

