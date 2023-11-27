#include "pch.h"
#include "Shader.h"

HRESULT VertexShader::CreateVertexShader(LPCWSTR _ShaderName, LPCSTR _EntryPoint, LPCSTR _ShaderModel)
{
    // Shader를 컴파일 해서 Binary Large Object file로 만들기
    bool Result = CompileShaderFromFile(_ShaderName, _EntryPoint, _ShaderModel, &m_pVSBlob);
    if (Result == false)
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return E_FAIL;
    }

    // 버텍스 쉐이더 객체 만들기
    HRESULT hr = g_pd3dDevice->CreateVertexShader(
        m_pVSBlob->GetBufferPointer(), 
        m_pVSBlob->GetBufferSize(), 
        nullptr, 
        &m_pVertexShader);
    if (FAILED(hr))
    {
        m_pVSBlob->Release();
        m_pVSBlob = nullptr;
        return hr;
    }
    return hr;
}

HRESULT VertexShader::SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* _layoutDesc, UINT _numElements)
{
    HRESULT hr = g_pd3dDevice->CreateInputLayout(
        _layoutDesc, _numElements, m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(), &m_pLayout);
    return hr;
}

VertexShader::VertexShader()
    : m_pVSBlob(nullptr)
    , m_pVertexShader(nullptr)
    , m_pLayout(nullptr)
{
}

VertexShader::~VertexShader()
{
    if (m_pVertexShader) m_pVertexShader->Release();
    if (m_pVSBlob) m_pVSBlob->Release();
    if (m_pLayout) m_pLayout->Release();
}

HRESULT PixelShader::CreatePixelShader(LPCWSTR _ShaderName, LPCSTR _EntryPoint, LPCSTR _ShaderModel)
{
    // 픽셀 쉐이더 컴파일 해주기
    bool Result = CompileShaderFromFile(_ShaderName, _EntryPoint, _ShaderModel, &m_pPSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return E_FAIL;
    }

    // 픽셀 쉐이더 만들기
    HRESULT hr = g_pd3dDevice->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
    return hr;
}

PixelShader::PixelShader()
    : m_pPSBlob(nullptr)
    , m_pPixelShader(nullptr)
{
}

PixelShader::~PixelShader()
{
    if (m_pPixelShader) m_pPixelShader->Release();
    if (m_pPSBlob) m_pPSBlob->Release();
}
