#include "pch.h"
#include "CRenderAsset.h"

D3D11_INPUT_ELEMENT_DESC DefaultLayout[] =
{
    {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //{"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

UINT DefaultLayoutNumElements = ARRAYSIZE(DefaultLayout);

void CRenderAsset::UpdateWorldMat()
{
    // 행렬 곱 순서는 스케일 -> 자전 -> 공전 (이동 + 회전) 순서로 곱하면 된다.
    m_WorldMat = MatrixIdentity();

    Matrix ScaleMat = MatrixScale(GetWorldTransform().Scale);

    Matrix RotMat = MatrixRotationZ(GetWorldTransform().Rotation.z);
    RotMat *= MatrixRotationX(GetWorldTransform().Rotation.x);
    RotMat *= MatrixRotationX(GetWorldTransform().Rotation.y);

    Matrix TransMat = MatrixTranslation(GetWorldTransform().Position);

    m_WorldMat = ScaleMat * RotMat * TransMat;
}

void CRenderAsset::UpdateRenderMat()
{
    //1. RenderAsset들을 전부 가지고 있는 RenderManager
    //2. 여기서 일단, 불투명한 애들을 냅다 출력한다.
    //3. 그리고 alpha가 < 0 인 친구들을 sort 한다.
    //4. 그리고 출력한다.
    UpdateWorldMat();

    m_RenderMat = m_WorldMat * g_ViewMat * g_ProjectionMat;
}

CRenderAsset::CRenderAsset()
    :CObject()
    , m_WorldMat(MatrixIdentity())
    , m_RenderMat(MatrixIdentity())
{
}

CRenderAsset::CRenderAsset(const CRenderAsset& _other)
    :CObject(_other)
    , m_WorldMat(MatrixIdentity())
    , m_RenderMat(MatrixIdentity())
{
    // 여기다가는 그 인스턴싱 어떻게 시도해보기
}

CRenderAsset::~CRenderAsset()
{
}
