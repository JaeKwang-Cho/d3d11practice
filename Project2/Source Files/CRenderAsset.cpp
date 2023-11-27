#include "pch.h"
#include "CRenderAsset.h"
#include "SimplePrimitives.h"
#include "ObjectRenderComp.h"

DefaultVertex DefaultCube[] =
{
    // Pos, Color, Norm, UV
    { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT2(0.0f, 0.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f) ,
    { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 0.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },
    { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f)},    //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },
    { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },

    { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT2(0.0f, 0.0f)}, //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
    { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
    { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(1.0f, 1.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
    { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 1.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },

    { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
    { FLOAT3(-1.0f, -1.0f, -1.0f),FLOAT2(1.0f, 0.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
    { FLOAT3(-1.0f, 1.0f, -1.0f),FLOAT2(1.0f, 1.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
    { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },

    { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
    { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
    { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 1.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
    { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f)},    //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },

    { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT2(0.0f, 0.0f)}, //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
    { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
    { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 1.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
    { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT2(0.0f, 1.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },

    { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f)},  //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f) },
    { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(1.0f, 0.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f) },
    { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f)},    //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f) },
    { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f)},   //FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f) },
};

WORD DefaultIndices[] =
{
    3,1,0,
    2,1,3,

    6,4,5,
    7,4,6,

    11,9,8,
    10,9,11,

    14,12,13,
    15,12,14,

    19,17,16,
    18,17,19,

    22,20,21,
    23,20,22
};

D3D11_INPUT_ELEMENT_DESC DefaultLayout[] =
{
    {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //{"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //{"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}

};

void CRenderAsset::StartObject()
{
    m_cube = new ObjectRenderComp;

    HRESULT hr;
    hr = m_cube->CreateVertexShader(L"DefaultShader.fx", "VS", "vs_5_0");

    UINT numElements = ARRAYSIZE(DefaultLayout);
    hr = m_cube->SetInputLayout(DefaultLayout, numElements);

    hr = m_cube->CreatePixelShader(L"DefaultShader.fx", "PS", "ps_5_0");

    numElements = ARRAYSIZE(DefaultCube);
    hr = m_cube->CreateVertexBuffer(DefaultCube, numElements);

    numElements = ARRAYSIZE(DefaultIndices);
    hr = m_cube->CreateIndexBuffer(DefaultIndices, numElements);

    hr = m_cube->CreateTextureResourceView(L"seafloor.dds");
    hr = m_cube->CreateDefaultTextureSampler();

    if (FAILED(hr))
    {
        assert("Initialize m_cube failed" && false);
        return;
    }

    SetWorldMat();
}

void CRenderAsset::UpdateObject()
{
    // Update our time
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)PI * 0.0125f;
    }
    else
    {
        static ULONGLONG dwTimeStart = 0;
        ULONGLONG dwTimeCur = GetTickCount64();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    m_cube->Update();

    SetWorldMat();
    // ���� ��Ű��
    //g_WorldMat = MatrixRotationY(t);
}

void CRenderAsset::RenderObject()
{
    // �� ������ ���� ���ϴ� constant buffer ������Ʈ �ϱ�
    CBChangesEveryFrame cb;
    cb.mWorld = MatrixTranspose(m_WorldMat);
    // ���̴����� ����� Constant buffer ��ü�� �ý��� �޸� ���� ī�����ش�.
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    // ���� GPU�� �Ͽ��� ���̴��� ����, �������� ��ȣ�� Contant buffer�� ���� ���� �ϴ��� �˷��ְ�
    // ����� ������ �ε����� ���� �ﰢ���� �׸����� ��Ų��.

    // m_cube

    {
        {
            g_pImmediateContext->VSSetShader(m_cube->GetVertexShader(), NULL, 0);
            g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBNeverChanges);
            g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangeOnResize);
            g_pImmediateContext->VSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
        }

        // Vectex shader�� constant buffer�� ������ �߾, Pixel Shader���� ���� ������ �ؾ� �Ѵ�.
        {
            g_pImmediateContext->PSSetShader(m_cube->GetPixelShader(), NULL, 0);
            g_pImmediateContext->PSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
            g_pImmediateContext->PSSetShaderResources(0, 1, m_cube->GetResourceView());
            // �׸��� sampler state�� ������ ���ش�.
            g_pImmediateContext->PSSetSamplers(0, 1, m_cube->GetSampler());
        }
        // ���� �׸���.
        g_pImmediateContext->DrawIndexed(m_cube->GetNumOfIndices(), 0, 0);
    }
}

void CRenderAsset::SetWorldMat()
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

CRenderAsset::CRenderAsset()
    :CObject()
    , m_cube(nullptr)
    , m_WorldMat(MatrixIdentity())
{
}

CRenderAsset::CRenderAsset(const CRenderAsset& _other)
    :CObject(_other)
    ,m_cube(nullptr)
    ,m_WorldMat(MatrixIdentity())
{
}

CRenderAsset::~CRenderAsset()
{
    if (m_cube) delete m_cube;
}
