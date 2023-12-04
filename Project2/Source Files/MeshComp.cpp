#include "pch.h"
#include "MeshComp.h"

HRESULT MeshComp::Initialize(vector<DefaultVertex>& _vertices, vector<WORD>& _indices, vector<TextureComp>& _textures, bool _bAlphaLessOne, Matrix _SubPosMat)
{
	m_Vertices = _vertices;
	m_Indices = _indices;
    m_Textures = _textures;
    m_bAlphaLessOne = _bAlphaLessOne;
    m_SubPosMat = _SubPosMat;

    HRESULT hr = S_OK;
    // Vertex Shader
    {
        ID3DBlob** ppVSBlob = &(m_VertexShader.m_pVSBlob);
        ID3D11VertexShader** ppVertexShader = &(m_VertexShader.m_pVertexShader);
        ID3D11InputLayout** ppLayout = &(m_VertexShader.m_pLayout);

        // 일단은 기본 쉐이더로 만들기
        bool Result = CompileShaderFromFile(L"DefaultShader.fx", "VS", "vs_5_0", ppVSBlob);
        if (Result == false)
        {
            MessageBox(nullptr,
                       L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
            return hr;
        }

        // 버텍스 쉐이더 객체 만들기
        hr = g_pd3dDevice->CreateVertexShader(
            (*ppVSBlob)->GetBufferPointer(),
            (*ppVSBlob)->GetBufferSize(),
            nullptr,
            ppVertexShader);
        if (FAILED(hr))
        {
            (*ppVSBlob)->Release();
            (*ppVSBlob) = nullptr;
            assert("MeshComp::Initialize - CreateVertexShader failed" && false);
            return hr;
        }
        (*ppVertexShader)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("MeshComp::Initialize - CreateVertexShader") - 1, "MeshComp::Initialize - CreateVertexShader");

        // Vertex Input Layout

        hr = g_pd3dDevice->CreateInputLayout(
            g_DefaultLayout, g_DefaultLayoutNumElements, (*ppVSBlob)->GetBufferPointer(),
            (*ppVSBlob)->GetBufferSize(), ppLayout);
        (*ppLayout)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("MeshComp::Initialize - CreateInputLayout") - 1, "MeshComp::Initialize - CreateInputLayout");
    }
    // Pixel Shader
    {
        ID3DBlob** ppPSBlob = &(m_PixelShader.m_pPSBlob);
        ID3D11PixelShader** ppPixelShader = &(m_PixelShader.m_pPixelShader);

        // 일단 기본 쉐이더로 해주기
        bool Result = CompileShaderFromFile(L"DefaultShader.fx", "PS", "ps_5_0", ppPSBlob);
        if (Result == false)
        {
            MessageBox(NULL,
                       L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
            return hr;
        }

        // 픽셀 쉐이더 만들기
        hr = g_pd3dDevice->CreatePixelShader((*ppPSBlob)->GetBufferPointer(), (*ppPSBlob)->GetBufferSize(), nullptr, ppPixelShader);
        if (FAILED(hr))
        {
            assert("MeshComp::Initialize - CreatePixelShader failed" && false);
        }

        (*ppPixelShader)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("MeshComp::Initialize - CreatePixelShader") - 1, "MeshComp::Initialize - CreatePixelShader");
    }

    // Vertex Buffer
    {
        // 일단 DefaultVertex로 한다.
        ID3D11Buffer** ppVertexBuffer = &(m_VertexBuffer.m_pVertexBuffer);
        m_VertexBuffer.m_stride = sizeof(DefaultVertex);
        m_VertexBuffer.m_offset = 0;

        D3D11_BUFFER_DESC vbd;
        memset(&vbd, 0, sizeof(vbd));
        vbd.Usage = D3D11_USAGE_DEFAULT;
        vbd.ByteWidth = m_VertexBuffer.m_stride * (UINT)m_Vertices.size();
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;

        // 버텍스 데이터 포인터를 연결해준다.
        D3D11_SUBRESOURCE_DATA InitData;
        memset(&InitData, 0, sizeof(InitData));
        InitData.pSysMem = m_Vertices.data();

        hr = g_pd3dDevice->CreateBuffer(&vbd, &InitData, ppVertexBuffer);
        if (FAILED(hr))
        {
            assert("MeshComp::Initialize - CreateVertexBuffer failed" && false);
            return hr;
        }

        (*ppVertexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("MeshComp::Initialize - CreateVertexShader - CreateVertexBuffer") - 1, "MeshComp::Initialize - CreateVertexShader - CreateVertexBuffer");

    }

    // Index Buffer
    {
        ID3D11Buffer** ppIndexBuffer = &(m_IndexBuffer.m_pIndexBuffer);
        m_IndexBuffer.m_NumOfIndices = (UINT)m_Indices.size();

        // 인덱스 버퍼 속성 정해주기
        D3D11_BUFFER_DESC ibd;
        memset(&ibd, 0, sizeof(ibd));
        ibd.Usage = D3D11_USAGE_DEFAULT;
        ibd.ByteWidth = sizeof(WORD) * m_IndexBuffer.m_NumOfIndices;
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;

        // 인덱스 데이터 포인터 연결해주기
        D3D11_SUBRESOURCE_DATA InitData;
        memset(&InitData, 0, sizeof(InitData));
        InitData.pSysMem = m_Indices.data();

        // 인덱스 버퍼 만들어주기
        hr = g_pd3dDevice->CreateBuffer(&ibd, &InitData, ppIndexBuffer);
        if (FAILED(hr))
        {
            assert("MeshComp::Initialize - CreateIndexBuffer failed" && false);
            return hr;
        }

        (*ppIndexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("MeshComp::Initialize - CreateIndexBuffer - CreateVertexBuffer") - 1, "MeshComp::Initialize - CreateIndexBuffer - CreateVertexBuffer");
    }

    return hr;
}

void MeshComp::UpdateComp(Matrix _RenderMat)
{
    m_RenderMat = m_SubPosMat * _RenderMat;
}

void MeshComp::CalcZValue()
{
    Vector4 vPos = VectorTransform(FLOAT4(m_Vertices[0].Pos, 1.f), m_RenderMat);
    m_fZval = vPos.z;        
}

void MeshComp::RenderComp()
{
    MVPMatrix cbMVPMatrix;
    cbMVPMatrix.mat = MatrixTranspose(m_RenderMat);
    g_pImmediateContext->UpdateSubresource(g_pCBMVPMat, 0, nullptr, &cbMVPMatrix, 0, 0);

    // constant buffer 전달하기
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBMVPMat);

    // 레이아웃 집어 넣기
    g_pImmediateContext->IASetInputLayout(m_VertexShader.m_pLayout);

    // 버텍스 버퍼 객체 세팅 (바꿀 일이 없이 때문에 이렇게 한다.)
    UINT stride = m_VertexBuffer.m_stride;
    UINT offset = m_VertexBuffer.m_offset;
    g_pImmediateContext->IASetVertexBuffers(0, 1, &m_VertexBuffer.m_pVertexBuffer, &stride, &offset);

    // Render Asset으로 빼기
    // 인덱스 버퍼 세팅 (인덱스 개수가 엄청 많아지면 DXGI_FORMAT_R32_UINT로 바꾼다.)
    g_pImmediateContext->IASetIndexBuffer(m_IndexBuffer.m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // 이제 GPU로 하여금 셰이더에 적힌, 레지스터 번호로 Contant buffer가 어디로 들어가야 하는지 알려주고
    // 계산을 돌리고 인덱스를 따라 삼각형을 그리도록 시킨다.
    {
        g_pImmediateContext->VSSetShader(m_VertexShader.m_pVertexShader, NULL, 0);
    }
    // Vectex shader에 constant buffer를 세팅을 했어도, Pixel Shader에도 따로 세팅을 해야 한다.
    {
        g_pImmediateContext->PSSetShader(m_PixelShader.m_pPixelShader, NULL, 0);
        g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pCBMVPMat);

        size_t i = 0;
        for (; i < m_Textures.size(); i++)
        {
            if (m_Textures[i].m_type == aiTextureType_DIFFUSE)
            {
                g_pImmediateContext->PSSetShaderResources(0, 1, &m_Textures[i].m_pTextureResourceView);
                break;
            }
        }
    }

    // 마저 그린다.
    g_pImmediateContext->DrawIndexed(m_IndexBuffer.m_NumOfIndices, 0, 0);
}

MeshComp::MeshComp()
	: m_Vertices()
	, m_Indices()
    , m_Textures()
    , m_bAlphaLessOne(false)
    , m_fZval(0.f)
    , m_SubPosMat()
    , m_RenderMat()
{
}

MeshComp::~MeshComp()
{
}
