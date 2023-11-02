#include "pch.h"
#include "D3DHelperFunc.h"

// Init ID3D11Device
bool InitDevice()
{
    HRESULT hr = S_OK;

    // �ϴ� �������� ������Ƽ�� ��������
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT    width = rc.right - rc.left;
    UINT    height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    // üũ�� ����̹� Ÿ�԰�
    D3D_DRIVER_TYPE driverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    // ���� ������ �迭�� ����� ����
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // ���� ü�� ��ũ���̺긦 �����Ѵ�.
    DXGI_SWAP_CHAIN_DESC sd;
    memset(&sd, 0, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ���� ���� �����ϴ�, ��ȣ ���� 4���� ���� sRGB(+A) ���
    sd.BufferDesc.RefreshRate.Numerator = 60; // Hz
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Surface�� Resource�� ��� ������� ����ϰڴٴ� ���̴�.
    sd.OutputWindow = g_hWnd; //��� �����츦 �����ְ�
    sd.SampleDesc.Count = 1; // ��Ƽ ���ø� ������Ƽ�� �����ְ�
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true; // â���� �������ְ�

    for (UINT i = 0; i < numDriverTypes; i++)
    {
        g_driverType = driverTypes[i];
        // �� Ŀ�ٶ� �Լ� �ϳ����� ����̹� Ÿ�԰� ���ķ����� Ȯ���ϸ鼭
        // ������Ƽ�� ����ü��, ����̽�, ���ķ���, ����̽� ���ؽ�Ʈ�� �����Ѵ�.
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr, g_driverType, nullptr, createDeviceFlags,
            featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
            &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext
        ); // ������ ����̽� ������ ���� ü�� ������ ���� �̷�����, �װ��� ���߿� �����ϰ� �ȴ�.
        if (SUCCEEDED(hr))
        {
            break;
        }
    }
    if (FAILED(hr))
    {
        return false;
    }

    // ���� Ÿ�� ��, ���̴� ���ҽ� �� �� �� �ϳ��� �����ϴ�.(�б� ����)
    // �̷� �� ���� ���ҽ��� ����ϱ� ���� �Ļ� �������̽���
    // ������� ģ���̴�. (�ٿ��� �ؽ����̴�. �ؽ��Ĵ� ���Ⱑ �����ϴ�.)
    ID3D11Texture2D* pBackBuffer = nullptr;
    // ����ü���� �����ϸ� �⺻������ ������ �ؽ��ĸ� ������ �ش�. (���е�� Desktop Window Manager���� �����ϴ� �ְ� ����Ʈ�̴�.)
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    // Texture2D �� ���۷� ����, ���� Ÿ������ ����� �� �ִ� Comptr�� �ϳ� ����� �ȴ�. ���ҽ��� �Ȱ��� ���� ����Ų��.
    hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
    {
        return false;
    }

    // depth stencil texture �� �����.
    D3D11_TEXTURE2D_DESC descDepth;
    memset(&descDepth, 0, sizeof(descDepth));
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    // ���� ���۵� �ؽ��ĸ� �ϴ� ���� ����ϴ� ���̴�.
    hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
    if (FAILED(hr))
    {
        return false;
    }

    // depth stencil view�� �����.
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSv;
    memset(&descDSv, 0, sizeof(descDSv));
    descDSv.Format = descDepth.Format;
    descDSv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSv.Texture2D.MipSlice = 0;
    // ������ ���� �ؽ��ķ� ���� ���ٽ� �並 ����� ��.
    hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSv, &g_pDepthStencilView);
    if (FAILED(hr))
    {
        return false;
    }

    // Render Target View�� Depth Stencil View�� Output-Merge ���·� ���ε� �Ѵ�.
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

    // ����Ʈ�� �����Ѵ�.
    D3D11_VIEWPORT vp;
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    // ����Ʈ ����ü�� �����Ͷ����� �ܰ迡 ���ε� �Ѵ�.
    g_pImmediateContext->RSSetViewports(1, &vp);

    return true;
}

bool CompileShaderFromFile(const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr;

    BYTE* data = new BYTE[SHADER_BUFFER_SIZE];
    FILE* file = nullptr;
    _wfopen_s(&file, szFileName, L"rb");
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    unsigned long iDataSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fread(data, sizeof(BYTE), (size_t)iDataSize, file);
    data[(size_t)iDataSize] = '\0';

    fclose(file);

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // <d3dcompiler.h>
#if defined(DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
    // Blob�� Binary Large Object�� ���Ѵ�.
    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompile2(data, (size_t)iDataSize, nullptr, nullptr, nullptr, szEntryPoint, szShaderModel, 0, 0, 0, 0, 0, ppBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob != NULL)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        if (pErrorBlob)
            pErrorBlob->Release();
        return false;
    }
    if (pErrorBlob)
    {
        pErrorBlob->Release();
    }

    return true;
}

void CleanupDevice()
{
    // Context���� �������� �� ������ ������
    if (g_pImmediateContext)
    {
        g_pImmediateContext->ClearState();
    }

    // ��״� �� COM ��ü ���̿��� �̷��� ������ ����� �Ѵ�.
    // �����ϴ� ������ �� �𸣰ڴ�.
    // Common
    if (g_pDepthStencil) g_pDepthStencil->Release();
    if (g_pDepthStencilView) g_pDepthStencilView->Release();
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pImmediateContext) g_pImmediateContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

    // Texutre
    if (g_pSamplerLinear) g_pSamplerLinear->Release();
    if (g_pTextureRV) g_pTextureRV->Release();
    if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
    if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
    if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
    // Cube
    if (g_pConstantBuffer) g_pConstantBuffer->Release();
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pIndexBuffer) g_pIndexBuffer->Release();
    if (g_pVertexLayout) g_pVertexLayout->Release();
    // Light
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pPixelShaderSolid) g_pPixelShaderSolid->Release();
    if (g_pPixelShader) g_pPixelShader->Release();

}