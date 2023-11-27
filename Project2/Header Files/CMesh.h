#pragma once
#include "CRenderAsset.h"

class CMesh :
    public CRenderAsset
{
private:

public:
    virtual void StartObject() override;

    virtual void UpdateObject() override;
    virtual void RenderObject() override;

public:
    CMesh(vector<DefaultVertex>& _vertices, vector<WORD>& _indices);
    CMesh(const CMesh& _other);
    virtual ~CMesh();
};

