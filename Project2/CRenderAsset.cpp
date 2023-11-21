#include "pch.h"
#include "CRenderAsset.h"
#include "SimplePrimitives.h"

void CRenderAsset::StartObject()
{
    // Default
    bool Result = GetSimpleCube(g_pd3dDevice,
                  g_pImmediateContext,
                  &m_VS,
                  &m_VertexBuff,
                  &m_Layout,
                  &m_PS,
                  &m_IndexBuff,
                  L"seafloor.dds",
                  &m_texRV,
                  &m_SamplerLinear);
    if (Result == false)
    {
        assert(false && "GetSimpleCube Failed");
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

    SetWorldMat();
    // 자전 시키기
    //g_WorldMat = MatrixRotationY(t);
}

void CRenderAsset::RenderObject()
{
    // 매 프레임 마다 변하는 constant buffer 업데이트 하기
    CBChangesEveryFrame cb;
    cb.mWorld = MatrixTranspose(m_WorldMat);
    // 쉐이더에서 사용할 Constant buffer 객체에 시스템 메모리 값을 카피해준다.
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    // 이제 GPU로 하여금 셰이더에 적힌, 레지스터 번호로 Contant buffer가 어디로 들어가야 하는지 알려주고
    // 계산을 돌리고 인덱스를 따라 삼각형을 그리도록 시킨다.
    {
        g_pImmediateContext->VSSetShader(m_VS, NULL, 0);
        g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBNeverChanges);
        g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangeOnResize);
        g_pImmediateContext->VSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
    }


    // Vectex shader에 constant buffer를 세팅을 했어도, Pixel Shader에도 따로 세팅을 해야 한다.
    {
        g_pImmediateContext->PSSetShader(m_PS, NULL, 0);
        g_pImmediateContext->PSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
        g_pImmediateContext->PSSetShaderResources(0, 1, &m_texRV);
        // 그리고 sampler state도 설정을 해준다.
        g_pImmediateContext->PSSetSamplers(0, 1, &m_SamplerLinear);
    }

    // 마저 그린다.
    g_pImmediateContext->DrawIndexed(36, 0, 0);
}

void CRenderAsset::SetWorldMat()
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

CRenderAsset::CRenderAsset()
    :CObject()
    , m_VS(nullptr)
    , m_VertexBuff(nullptr)
    , m_Layout(nullptr)
    , m_PS(nullptr)
    , m_IndexBuff(nullptr)
    , m_texFile(nullptr)
    , m_texRV(nullptr)
    , m_SamplerLinear(nullptr)
    , m_WorldMat(MatrixIdentity())
{
}

CRenderAsset::~CRenderAsset()
{
    if (m_VS) m_VS->Release();
    if( m_VertexBuff) m_VertexBuff->Release();
    if( m_Layout) m_Layout->Release();
    if( m_PS) m_PS->Release();
    if( m_IndexBuff) m_IndexBuff->Release();
    if( m_texRV) m_texRV->Release();
    if( m_SamplerLinear) m_SamplerLinear->Release();
}
