#pragma once
#include "CObject.h"

class CActor :
    public CObject
{
private:


private:
	Matrix m_PosMat;
	Matrix m_RotMat;
	Matrix m_ScalMat;

	FLOAT4 m_vPosition;
	FLOAT3 m_vRotation;
	FLOAT3 m_vScale;

public:
	void SetPos(float _x, float _y, float _z);
	void SetPos(FLOAT4 _pos);

	void SetRotation(float _xAxis, float _yAxis, float _zAxis, ROT_ORDER _order = ROT_ORDER::ZYX);
	void SetRotation(FLOAT3 _Rot, ROT_ORDER _order = ROT_ORDER::ZYX);

	void SetScale(float _xs, float _ys, float _zs);
	void SetScale(FLOAT3 _Scale);


public:
	virtual void StartObject() override;

	virtual void UpdateObject() override;

	virtual void RenderObject() override;

	virtual CObject* Clone()  override;

public:
	CActor();
	virtual ~CActor();
};

