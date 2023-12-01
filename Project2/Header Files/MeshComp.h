#pragma once

#include "CRenderAsset.h"
#include "ObjectRenderComp.h"
#include "TextureComp.h"

struct MeshComp
{
    ObjectRenderComp* m_RenderComp; 
    vector<DefaultVertex> m_Vertices;
    vector<WORD> m_Indices;
    vector<TextureComp> m_Textures;
    bool m_bAlphaLessOne;

public:
    void Initialize(vector<DefaultVertex>& _vertices, vector<WORD>& _indices, vector<TextureComp>& _textures, bool _bAlphaLessOne);

    void StartRender();

    void Render();

public:
    MeshComp();
    virtual ~MeshComp();
};
