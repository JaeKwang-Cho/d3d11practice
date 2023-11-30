#include "pch.h"
#include "D3DHelperFunc.h"
#include "dxgidebug.h"

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

    g_pSwapChain->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pSwapChain") - 1, "g_pSwapChain");
    g_pd3dDevice->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pd3dDevice") - 1, "g_pd3dDevice");
    g_pImmediateContext->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pImmediateContext") - 1, "g_pImmediateContext");


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
    g_pRenderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pRenderTargetView") - 1, "g_pRenderTargetView");

    // rasterizer ��带 �����Ѵ�.
    // https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc
    D3D11_RASTERIZER_DESC rd;
    memset(&rd, 0, sizeof(rd));
    rd.FillMode = D3D11_FILL_SOLID; // �׳� ������ (�������� ���̾�������)
    rd.CullMode = D3D11_CULL_BACK; // �� �ﰢ���� ¥����.
    rd.FrontCounterClockwise = false; // �ݽð� ������ ������ ( +z�� �����̴ϱ�)
    rd.DepthBias = 0; // ���� ���̾ (������ Z �� ������, �� �� ��������(�������� �� �ߵǰ�) �ϴ� ģ���̴�.
    rd.DepthBiasClamp = 0.f; // https://learn.microsoft.com/ko-kr/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
    rd.SlopeScaledDepthBias = 0.f;
    rd.DepthClipEnable = false; // �̰͵� �ణ �����ϴ�.
    rd.ScissorEnable = false; // https://learn.microsoft.com/ko-kr/windows/win32/direct3d9/scissor-test
    rd.MultisampleEnable = false;
    rd.AntialiasedLineEnable = false;
   
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRasterizerState);
    if (FAILED(hr))
    {
        assert("g_pd3dDevice->CreateRasterizerState" && false);
        return false;
    }
    g_pImmediateContext->RSSetState(g_pRasterizerState);
    g_pRasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pRasterizerState") - 1, "g_pRasterizerState");

    // depth / stencil ��带 �����Ѵ�.
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_depth_stencil_desc
    D3D11_DEPTH_STENCIL_DESC dsd;
    memset(&dsd, 0, sizeof(dsd));
    dsd.DepthEnable = true;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc = D3D11_COMPARISON_LESS;
    dsd.StencilEnable = false;
    dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    // depth-stencil ���� ���� ���� ���� ��
    dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // depth-stencil ���� �ڸ� ���� ���� ��
    dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // �ϴ��� �⺻ �����̴�.
    g_pd3dDevice->CreateDepthStencilState(&dsd, &g_pDepthStencilState);
    g_pImmediateContext->OMSetDepthStencilState(g_pDepthStencilState, 0);

    g_pDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pDepthStencilState") - 1, "g_pDepthStencilState");

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
    g_pDepthStencil->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pDepthStencil") - 1, "g_pDepthStencil");

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
    g_pDepthStencilView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pDepthStencilView") - 1, "g_pDepthStencilView");

    // Render Target View�� Depth Stencil View�� Output-Merge ���·� ���ε� �Ѵ�.
    // OM�� ��� : ����Ÿ�ٿ��ٰ� ������ ���������ο��� ���� �ȼ� ���� �׸��� ��
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
    // ���� Ÿ���� �����ϴ� ����� ������ ����.
    // g_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

    
    // Blend ��带 �����Ѵ�.
    // https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_blend_desc
    D3D11_BLEND_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.AlphaToCoverageEnable = false;
    bd.IndependentBlendEnable = false;

    // 1) �⺻ ����
    //bd.RenderTarget[0].BlendEnable = false; // �ִ� 8�� ���� �ִ�.
    //bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Blend RenderTarget�� �����Ѵ�. (�ִ� 8�� ���� �ִ�)
    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    memset(&rtbd, 0, sizeof(rtbd));
    rtbd.BlendEnable = true;
    rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // 2) ���� Ÿ�� ���� �߰� ����
    bd.RenderTarget[0] = rtbd;

    g_pd3dDevice->CreateBlendState(&bd, &g_pBlendState);
    g_pBlendState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pBlendState") - 1, "g_pBlendState");
    
    float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
    g_pImmediateContext->OMSetBlendState(g_pBlendState, nullptr, 0xffffffff);

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
    if (g_pRasterizerState) g_pRasterizerState->Release();
    if (g_pBlendState) g_pBlendState->Release();
    if (g_pDepthStencilState) g_pDepthStencilState->Release();
    if (g_pDepthStencil) g_pDepthStencil->Release();
    if (g_pDepthStencilView) g_pDepthStencilView->Release();
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pImmediateContext) g_pImmediateContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

}

void CleanupSamples()
{
    // Context���� �������� �� ������ ������
    if (g_pImmediateContext)
    {
        g_pImmediateContext->ClearState();
    }

    // ��״� �� COM ��ü ���̿��� �̷��� ������ ����� �Ѵ�.
    // �����ϴ� ������ �� �𸣰ڴ�.
    // Common
    if (g_pRasterizerState) g_pRasterizerState->Release();
    if (g_pBlendState) g_pBlendState->Release();
    if (g_pDepthStencilState) g_pDepthStencilState->Release();
    if (g_pDepthStencil) g_pDepthStencil->Release();
    if (g_pDepthStencilView) g_pDepthStencilView->Release();
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pImmediateContext) g_pImmediateContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();

}

// �ڵ忡�� ������ ����� �ٲٰ� ������ ȣ���ϴ� ��
HRESULT ResizeWindow()
{
    if (g_pSwapChain)
    {
        DXGI_MODE_DESC gd;
        gd.Width = 1280;
        gd.Height = 768;
        gd.RefreshRate.Numerator = 60;
        gd.RefreshRate.Denominator = 1;
        gd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        gd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        gd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        return g_pSwapChain->ResizeTarget(&gd);
    }
    return S_OK;
}

void CallbackResizeWindow()
{
    if (g_pImmediateContext && g_pSwapChain && g_pCBChangeOnResize)
    {
        g_pSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

        RECT rc = {};
        GetClientRect(g_hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;
        g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv2, width / (float)height, 0.01f, 100.f);

        // ������ ũ�� ��ȯ �ÿ� ���ϴ� Constant buffer �ʱ�ȭ
        CBChangeOnResize cbChangeOnResize;
        cbChangeOnResize.mProjection = MatrixTranspose(g_ProjectionMat);
        g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangeOnResize, 0, 0);
    }
}

void GetShaderResourceFromViewToFile(ID3D11ShaderResourceView* _texResourceView)
{
    // ���̴� ���ҽ� �並 ���� ���ҽ��� ���
    ID3D11Resource* pResource = nullptr;
    _texResourceView->GetResource(&pResource);

    // �ؽ��� ���� �������̽� ���
    D3D11_RESOURCE_DIMENSION resourceType;
    pResource->GetType(&resourceType);

    switch (resourceType)
    {
    case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
    {
        ID3D11Texture1D* pTexture1D = (ID3D11Texture1D*)pResource;
    }
    break;
    case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
    {
        ScratchImage scratchImage = ScratchImage{};
        CaptureTexture(g_pd3dDevice, g_pImmediateContext, (ID3D11Texture2D*)pResource, scratchImage);
        SaveToDDSFile(scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), DDS_FLAGS_NONE, L"Test.dds");
    }
    break;
    case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
    {
        ID3D11Texture3D* pTexture3D = (ID3D11Texture3D*)pResource;
    }
    break;
    default:
        assert("pResource is not Texture Resources" && false);
    }

    if (pResource) pResource->Release();
}

void GetShaderResourceFromViewToFile(ID3D11Resource* pResource)
{
    // �ؽ��� ���� �������̽� ���
    D3D11_RESOURCE_DIMENSION resourceType;
    pResource->GetType(&resourceType);

    switch (resourceType)
    {
    case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
    {
        ID3D11Texture1D* pTexture1D = (ID3D11Texture1D*)pResource;
    }
    break;
    case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
    {
        ScratchImage scratchImage = ScratchImage{};
        CaptureTexture(g_pd3dDevice, g_pImmediateContext, (ID3D11Texture2D*)pResource, scratchImage);
        SaveToDDSFile(scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), DDS_FLAGS_NONE, L"Test.dds");
    }
    break;
    case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
    {
        ID3D11Texture3D* pTexture3D = (ID3D11Texture3D*)pResource;
    }
    break;
    default:
        assert("pResource is not Texture Resources" && false);
    }
}

#ifdef _DEBUG
void FindD3DComObjLeak()
{
    HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
    decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

    IDXGIDebug* debug;

    GetDebugInterface(IID_PPV_ARGS(&debug));

    OutputDebugStringW(L"============== D3D Object ref count : COM leak check ==============\n");
    debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
    OutputDebugStringW(L"================ End of IUnKnown Object leaked ... ================\n");

    debug->Release();
}
#endif