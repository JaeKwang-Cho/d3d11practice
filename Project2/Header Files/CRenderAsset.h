#pragma once
#include "CObject.h"

extern D3D11_INPUT_ELEMENT_DESC DefaultLayout[];
extern UINT DefaultLayoutNumElements;

class CMesh;

class CRenderAsset :
    public CObject
{
protected:
	Matrix m_WorldMat;
	//ObjectRenderComp* m_cube;
private:

public:

public:
	virtual void StartObject() override;

	virtual void UpdateObject() override;
	virtual void RenderObject() override;

protected:
	void SetWorldMat();

public:
	CRenderAsset();
	CRenderAsset(const CRenderAsset& _other);
	virtual ~CRenderAsset();
};

