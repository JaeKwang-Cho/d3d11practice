#pragma once
#include "CScene.h"

class CDefault_Scene :
    public CScene
{

public:
    virtual void EnterScene() override;
    virtual void UpdateScene() override;
    virtual void Exit() override;

public:
    CDefault_Scene();
    virtual ~CDefault_Scene();
};

