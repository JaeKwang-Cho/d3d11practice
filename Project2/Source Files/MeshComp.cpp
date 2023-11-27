#include "pch.h"
#include "MeshComp.h"

void MeshComp::Initialize(vector<DefaultVertex>& _vertices, vector<WORD>& _indices)
{
	m_Vertices = _vertices;
	m_Indices = _indices;

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

    hr = m_RenderComp->CreateTextureResourceView(L"seafloor.dds");
    hr = m_RenderComp->CreateDefaultTextureSampler();

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
    if (m_RenderComp != nullptr)
    {
        m_RenderComp->Render(_WorldMat);
    }
}

MeshComp::MeshComp()
	: m_RenderComp(nullptr)
	, m_Vertices()
	, m_Indices()
{
}

MeshComp::~MeshComp()
{
    if (m_RenderComp) delete m_RenderComp;
}
