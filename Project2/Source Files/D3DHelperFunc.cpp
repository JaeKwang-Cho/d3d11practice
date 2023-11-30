#include "pch.h"
#include "D3DHelperFunc.h"
#include "dxgidebug.h"

// Init ID3D11Device
bool InitDevice()
{
    HRESULT hr = S_OK;

    // 일단 윈도우의 프로퍼티를 가져오고
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT    width = rc.right - rc.left;
    UINT    height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    // 체크할 드라이버 타입과
    D3D_DRIVER_TYPE driverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    // 피쳐 레벨을 배열로 만들어 놓고
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    // 스왑 체인 디스크라이브를 설정한다.
    DXGI_SWAP_CHAIN_DESC sd;
    memset(&sd, 0, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 알파 값을 포함하는, 부호 없는 4개의 정수 sRGB(+A) 방식
    sd.BufferDesc.RefreshRate.Numerator = 60; // Hz
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Surface나 Resource를 출력 대상으로 사용하겠다는 것이다.
    sd.OutputWindow = g_hWnd; //출력 윈도우를 정해주고
    sd.SampleDesc.Count = 1; // 멀티 샘플링 프로퍼티도 정해주고
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true; // 창모드로 설정해주고

    for (UINT i = 0; i < numDriverTypes; i++)
    {
        g_driverType = driverTypes[i];
        // 이 커다란 함수 하나에서 드라이버 타입과 피쳐레밸을 확인하면서
        // 프로퍼티로 스왑체인, 디바이스, 피쳐레벨, 디바이스 컨텍스트를 생성한다.
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr, g_driverType, nullptr, createDeviceFlags,
            featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
            &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext
        ); // 원래는 디바이스 생성과 스왑 체인 생성이 따로 이뤄지고, 그것을 나중에 결합하게 된다.
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


    // 렌더 타겟 뷰, 쉐이더 리소스 뷰 둘 중 하나로 가능하다.(읽기 전용)
    // 이런 뷰 들은 리소스를 사용하기 위한 파생 인터페이스로
    // 만들어진 친구이다. (근원은 텍스쳐이다. 텍스쳐는 쓰기가 가능하다.)
    ID3D11Texture2D* pBackBuffer = nullptr;
    // 스왑체인을 생성하면 기본적으로 나오는 텍스쳐를 백으로 준다. (프론드는 Desktop Window Manager에서 관리하는 애가 프론트이다.)
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    // Texture2D 백 버퍼로 부터, 랜더 타겟으로 사용할 수 있는 Comptr을 하나 만들게 된다. 리소스는 똑같은 것을 가리킨다.
    hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
    {
        return false;
    }
    g_pRenderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pRenderTargetView") - 1, "g_pRenderTargetView");

    // rasterizer 모드를 설정한다.
    // https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc
    D3D11_RASTERIZER_DESC rd;
    memset(&rd, 0, sizeof(rd));
    rd.FillMode = D3D11_FILL_SOLID; // 그냥 렌더링 (나머지는 와이어프레임)
    rd.CullMode = D3D11_CULL_BACK; // 뒤 삼각형을 짜른다.
    rd.FrontCounterClockwise = false; // 반시계 방향이 앞쪽임 ( +z가 뒤쪽이니깐)
    rd.DepthBias = 0; // 깊이 바이어스 (동일한 Z 가 있을때, 좀 더 앞쪽으로(렌더링이 더 잘되게) 하는 친구이다.
    rd.DepthBiasClamp = 0.f; // https://learn.microsoft.com/ko-kr/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias
    rd.SlopeScaledDepthBias = 0.f;
    rd.DepthClipEnable = false; // 이것도 약간 복잡하다.
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

    // depth / stencil 모드를 설정한다.
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_depth_stencil_desc
    D3D11_DEPTH_STENCIL_DESC dsd;
    memset(&dsd, 0, sizeof(dsd));
    dsd.DepthEnable = true;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc = D3D11_COMPARISON_LESS;
    dsd.StencilEnable = false;
    dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    // depth-stencil 면이 앞을 보고 있을 때
    dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // depth-stencil 면이 뒤를 보고 있을 때
    dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // 일단은 기본 상태이다.
    g_pd3dDevice->CreateDepthStencilState(&dsd, &g_pDepthStencilState);
    g_pImmediateContext->OMSetDepthStencilState(g_pDepthStencilState, 0);

    g_pDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pDepthStencilState") - 1, "g_pDepthStencilState");

    // depth stencil texture 를 만든다.
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
    // 뎁스 버퍼도 텍스쳐를 일단 만들어서 사용하는 것이다.
    hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
    if (FAILED(hr))
    {
        return false;
    }
    g_pDepthStencil->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pDepthStencil") - 1, "g_pDepthStencil");

    // depth stencil view를 만든다.
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSv;
    memset(&descDSv, 0, sizeof(descDSv));
    descDSv.Format = descDepth.Format;
    descDSv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSv.Texture2D.MipSlice = 0;
    // 위에서 만든 텍스쳐로 뎁스 스텐실 뷰를 만드는 것.
    hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSv, &g_pDepthStencilView);
    if (FAILED(hr))
    {
        return false;
    }
    g_pDepthStencilView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pDepthStencilView") - 1, "g_pDepthStencilView");

    // Render Target View와 Depth Stencil View를 Output-Merge 상태로 바인딩 한다.
    // OM의 기능 : 렌더타겟에다가 렌더링 파이프라인에서 계산된 픽셀 값을 그리는 것
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
    // 렌더 타겟을 해제하는 방법은 다음과 같다.
    // g_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

    
    // Blend 모드를 설정한다.
    // https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_blend_desc
    D3D11_BLEND_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.AlphaToCoverageEnable = false;
    bd.IndependentBlendEnable = false;

    // 1) 기본 상태
    //bd.RenderTarget[0].BlendEnable = false; // 최대 8개 까지 있다.
    //bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Blend RenderTarget을 설정한다. (최대 8개 까지 있다)
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

    // 2) 렌더 타겟 블랜드 추가 상태
    bd.RenderTarget[0] = rtbd;

    g_pd3dDevice->CreateBlendState(&bd, &g_pBlendState);
    g_pBlendState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("g_pBlendState") - 1, "g_pBlendState");
    
    float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
    g_pImmediateContext->OMSetBlendState(g_pBlendState, nullptr, 0xffffffff);

    // 뷰포트를 설정한다.
    D3D11_VIEWPORT vp;
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    // 뷰포트 구조체를 레스터라이즈 단계에 바인딩 한다.
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
    // Blob은 Binary Large Object를 말한다.
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
    // Context에서 연관성을 싹 없애준 다음에
    if (g_pImmediateContext)
    {
        g_pImmediateContext->ClearState();
    }

    // 얘네는 다 COM 객체 들이여서 이렇게 해제를 해줘야 한다.
    // 해제하는 순서는 잘 모르겠다.
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
    // Context에서 연관성을 싹 없애준 다음에
    if (g_pImmediateContext)
    {
        g_pImmediateContext->ClearState();
    }

    // 얘네는 다 COM 객체 들이여서 이렇게 해제를 해줘야 한다.
    // 해제하는 순서는 잘 모르겠다.
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

// 코드에서 윈도우 사이즈를 바꾸고 싶을때 호출하는 것
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

        // 윈도우 크기 변환 시에 변하는 Constant buffer 초기화
        CBChangeOnResize cbChangeOnResize;
        cbChangeOnResize.mProjection = MatrixTranspose(g_ProjectionMat);
        g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangeOnResize, 0, 0);
    }
}

void GetShaderResourceFromViewToFile(ID3D11ShaderResourceView* _texResourceView)
{
    // 셰이더 리소스 뷰를 통해 리소스를 얻기
    ID3D11Resource* pResource = nullptr;
    _texResourceView->GetResource(&pResource);

    // 텍스쳐 차원 인터페이스 얻기
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
    // 텍스쳐 차원 인터페이스 얻기
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