#pragma once

#include "CRenderAsset.h"
#include "TextureComp.h"
#include "BasicRenderComp.h"

struct MeshComp
{
    VertexShader m_VertexShader;
    PixelShader m_PixelShader;
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;

    vector<DefaultVertex> m_Vertices;
    vector<WORD> m_Indices;
    vector<TextureComp> m_Textures;
    bool m_bAlphaLessOne;
    float m_fZval;

public:
    HRESULT Initialize(vector<DefaultVertex>& _vertices, vector<WORD>& _indices, vector<TextureComp>& _textures, bool _bAlphaLessOne);

    void UpdateComp();

    void RenderComp();

public:
    MeshComp();
    virtual ~MeshComp();
};
