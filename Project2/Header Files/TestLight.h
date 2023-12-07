#pragma once
#include "CLight.h"

class CMesh;

class TestLight :
    public CLight
{
private:
    CMesh* m_lightmesh;

public:
    virtual void StartObject() override;
    virtual void UpdateObject() override;
    virtual CObject* Clone() override;

public:
    TestLight();
    virtual ~TestLight();
};

