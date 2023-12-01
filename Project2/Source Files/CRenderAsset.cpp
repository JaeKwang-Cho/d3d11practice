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
    // ��� �� ������ ������ -> ���� -> ���� (�̵� + ȸ��) ������ ���ϸ� �ȴ�.
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
    //1. RenderAsset���� ���� ������ �ִ� RenderManager
    //2. ���⼭ �ϴ�, �������� �ֵ��� ���� ����Ѵ�.
    //3. �׸��� alpha�� < 0 �� ģ������ sort �Ѵ�.
    //4. �׸��� ����Ѵ�.
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
    // ����ٰ��� �� �ν��Ͻ� ��� �õ��غ���
}

CRenderAsset::~CRenderAsset()
{
}
