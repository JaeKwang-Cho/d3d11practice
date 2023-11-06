#pragma once
#include "CActor.h"
class TextActor :
    public CActor
{
public:

    virtual void StartObject() override;
public:
    TextActor();
    virtual ~TextActor();
};

