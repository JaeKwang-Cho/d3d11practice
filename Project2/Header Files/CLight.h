#pragma once
#include "CObject.h"

class CLight
	: public CObject
{
protected:
	LightBuffer m_Light;

public:
	LightBuffer GetLightBuffer(){return m_Light;}
public:
	void SetWorldPosition(float _x, float _y, float _z)
	{
		m_WorldTransform.Position = FLOAT3(_x, _y, _z);
	}
	void SetWorldPosition(FLOAT3 _pos)
	{
		m_WorldTransform.Position = _pos;
	}

	void SetWorldRotation(float _xAxis, float _yAxis, float _zAxis)
	{
		m_WorldTransform.Rotation = FLOAT3(_xAxis, _yAxis, _zAxis);
	}
	void SetWorldRotation(FLOAT3 _Rot)
	{
		m_WorldTransform.Rotation = _Rot;
	}
	void SetWorldScale(float _xs, float _ys, float _zs)
	{
		m_WorldTransform.Scale = FLOAT3(_xs, _ys, _zs);
	}
	void SetWorldScale(FLOAT3 _Scale)
	{
		m_WorldTransform.Scale = _Scale;
	}


public:
	virtual void StartObject() = 0;
	virtual void UpdateObject() = 0;
	virtual CObject* Clone() = 0;

public:
	CLight();
	virtual ~CLight();
};

