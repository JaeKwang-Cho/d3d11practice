#pragma once
#include "CObject.h"
class CActor :
    public CObject
{
protected:
	FLOAT4 m_vPosition;
	FLOAT3 m_vRotation;
	FLOAT3 m_vScale;
public:



public:
	virtual void Start() override;

	virtual void UpdateObject() override;
	virtual void UpdateComponent() override;
	virtual void Render() override;

	virtual CObject* Clone()  override;

public:
	CActor();
	virtual ~CActor();
};

