#pragma once

#include "CRenderAsset.h"
#include "ObjectRenderComp.h"

struct MeshComp
{
    ObjectRenderComp* m_RenderComp; 
    vector<DefaultVertex> m_Vertices;
    vector<WORD> m_Indices;

public:
    void Initialize(vector<DefaultVertex>& _vertices, vector<WORD>& _indices);

    void StartRender();

    void Render(Matrix _WorldMat);

public:
    MeshComp();
    virtual ~MeshComp();
};
