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
	Matrix m_RenderMat;
	//ObjectRenderComp* m_cube;
private:

public:

public:
	virtual void StartObject() = 0;

	virtual void UpdateObject() = 0;
	virtual void RenderObject() = 0;

protected:
	void UpdateWorldMat();
	void UpdateRenderMat();

public:
	CRenderAsset();
	CRenderAsset(const CRenderAsset& _other);
	virtual ~CRenderAsset();
};

