#include "pch.h"
#include "MeshComp.h"

void MeshComp::Initialize(vector<DefaultVertex>& _vertices, vector<WORD>& _indices, vector<TextureComp>& _textures)
{
	m_Vertices = _vertices;
	m_Indices = _indices;
    m_Textures = _textures;

    m_RenderComp = new ObjectRenderComp;

    HRESULT hr;
    hr = m_RenderComp->CreateVertexShader(L"DefaultShader.fx", "VS", "vs_5_0");

    UINT numElements = DefaultLayoutNumElements;
    hr = m_RenderComp->SetInputLayout(DefaultLayout, numElements);

    hr = m_RenderComp->CreatePixelShader(L"DefaultShader.fx", "PS", "ps_5_0");

    numElements = (UINT)m_Vertices.size();
    hr = m_RenderComp->CreateVertexBuffer(m_Vertices.data(), numElements);

    numElements = (UINT)m_Indices.size();
    hr = m_RenderComp->CreateIndexBuffer(m_Indices.data(), numElements);

    //hr = m_RenderComp->CreateTextureResourceViewFromImage(L"seafloor.dds");
    //hr = m_RenderComp->CreateDefaultTextureSampler();

    if (FAILED(hr))
    {
        assert("Initialize m_Mesh failed" && false);
        return;
    }
}

void MeshComp::StartRender()
{
    if (m_RenderComp != nullptr)
    {
        m_RenderComp->StartRender();
    }
}

void MeshComp::Render(Matrix _WorldMat)
{
    // 매 프레임 마다 변하는 constant buffer 업데이트 하기
    CBChangesEveryFrame cb;
    cb.mWorld = MatrixTranspose(_WorldMat);
    // 쉐이더에서 사용할 Constant buffer 객체에 시스템 메모리 값을 카피해준다.
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    // 이제 GPU로 하여금 셰이더에 적힌, 레지스터 번호로 Contant buffer가 어디로 들어가야 하는지 알려주고
    // 계산을 돌리고 인덱스를 따라 삼각형을 그리도록 시킨다.
    {
        g_pImmediateContext->VSSetShader(m_RenderComp->GetVertexShader(), NULL, 0);
        g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBNeverChanges);
        g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangeOnResize);
        g_pImmediateContext->VSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
    }
    // Vectex shader에 constant buffer를 세팅을 했어도, Pixel Shader에도 따로 세팅을 해야 한다.
    {
        g_pImmediateContext->PSSetShader(m_RenderComp->GetPixelShader(), NULL, 0);
        g_pImmediateContext->PSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);

        size_t i = 0;
        for (; i < m_Textures.size(); i++)
        {
            if (m_Textures[i].GetTextureType() == aiTextureType_DIFFUSE)
            {
                g_pImmediateContext->PSSetShaderResources(0, 1, m_Textures[i].GetTextureResourceViewAddress());
                break;
            }
        }
    }
    // 마저 그린다.
    g_pImmediateContext->DrawIndexed(m_RenderComp->GetNumOfIndices(), 0, 0);
}

MeshComp::MeshComp()
	: m_RenderComp(nullptr)
	, m_Vertices()
	, m_Indices()
    , m_Textures()
{
}

MeshComp::~MeshComp()
{
    if (m_RenderComp) delete m_RenderComp;
}
