#pragma once
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
    Matrix m_SubPosMat;
    Matrix m_RenderMat;
    Matrix m_WorldMat;

public:
    HRESULT Initialize(vector<DefaultVertex>& _vertices, vector<WORD>& _indices, vector<TextureComp>& _textures, bool _bAlphaLessOne, Matrix _SubPosMat);

    void UpdateComp(Matrix _WorldMat);

    void CalcZValue();

    void RenderComp();

public:
    MeshComp();
    virtual ~MeshComp();
};
