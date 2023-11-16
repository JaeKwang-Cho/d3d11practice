#pragma once
#include "CActor.h"

class CRenderAsset;

class TestActor :
    public CActor
{
private:
    CRenderAsset* pRAs[1000];
public:

    virtual void StartObject() override;
    virtual void UpdateObject() override;
public:
    TestActor();
    virtual ~TestActor();
};

