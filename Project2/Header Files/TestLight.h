#pragma once
#include "CLight.h"

class CMesh;

class TestLight :
    public CLight
{
private:
    CMesh* m_lightmesh;

public:
    virtual void UpdateObject() override;

public:
    TestLight();
    virtual ~TestLight();
};

