#include "pch.h"
#include "D3DHelperFunc.h"

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

    // Render Target View와 Depth Stencil View를 Output-Merge 상태로 바인딩 한다.
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

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