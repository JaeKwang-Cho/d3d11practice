#pragma once
#include "CScene.h"
class CDefault_Scene :
    public CScene
{

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CDefault_Scene();
    virtual ~CDefault_Scene();
};

