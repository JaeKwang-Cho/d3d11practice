#pragma once
#include "CActor.h"

class TestActor :
    public CActor
{
public:

    virtual void StartObject() override;
public:
    TestActor();
    virtual ~TestActor();
};

