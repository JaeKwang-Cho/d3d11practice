#pragma once
#include "CActor.h"

class CMesh;

class TestActor :
    public CActor
{
private:
    CMesh* m_mesh;
    //CRenderAsset* pRAs[1000];
public:

    virtual void StartObject() override;
    virtual void UpdateObject() override;
public:
    TestActor();
    virtual ~TestActor();
};

