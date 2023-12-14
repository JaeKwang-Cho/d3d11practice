#pragma once
#include "CObject.h"
class CObject3D :
    public CObject
{
private:

public:
	virtual void StartObject() = 0;
	virtual void UpdateObject() = 0;
	virtual CObject* Clone() = 0;

protected:
    virtual void FindWorldTransform() override;

public:
       
};

