// project1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

#include <xnamath.h>

#include "framework.h"
#include "main.h"
#include "pch.h"

#include <dxgidebug.h>

#define MAX_LOADSTRING (100)
#define SHADER_BUFFER_SIZE (8192)

struct SimpleVertex
{
    FLOAT3 Pos;
    FLOAT4 Normal;
};

struct SimpleVertex_Norm 
{
    FLOAT3 POS;
    FLOAT3 Normal;
};

struct SimpleVertex_Tex
{
    FLOAT3 Pos;
    FLOAT2 TEX;
};

struct ConstantBuffer
{
    Matrix mWorld;
    Matrix mView;
    Matrix mProjection;
};

struct ConstantBuffer_Norm
{
    Matrix mWorld;
    Matrix mView;
    Matrix mProjection;
    FLOAT4 vLightDir[2];
    FLOAT4 vLightColor[2];
    FLOAT4 vOutputColor;
};

struct CBNeverChanges
{
    Matrix mView;
};

struct CBChangeOnResize
{
    Matrix mProjection;
};

struct CBChangesEveryFrame
{
    Matrix mWorld;
    FLOAT4 vMeshColor;
};


// =================
// ====전역  변수====
// =================
// Win32
HINSTANCE                   g_hInst = nullptr;  // 현재 인스턴스
HWND                        g_hWnd = nullptr;   // 윈도우 핸들
// d3d Attribute
D3D_DRIVER_TYPE             g_driverType = D3D_DRIVER_TYPE_NULL; // 드라이버 종류
D3D_FEATURE_LEVEL           g_featureLevel = D3D_FEATURE_LEVEL_11_1; // 사용할 directx 버전(?)
// d3d Property
ID3D11Device*               g_pd3dDevice = nullptr; // 리소스를 만들고, 디스플레이 어뎁터 기능을 사용하게 하는 것
ID3D11DeviceContext*        g_pImmediateContext = nullptr; // 디바이스로 하여금 파이프라인 상태를 제어하여, 렌더링 명령을 내리는데 사용한다.
IDXGISwapChain*             g_pSwapChain = nullptr; // 더블 버퍼링을 위한 Surface(IDXGISurface)를 구성하는데 사용한다.
ID3D11RenderTargetView*     g_pRenderTargetView = nullptr;
// Shader and Property
ID3D11Texture2D*            g_pDepthStencil = nullptr;
ID3D11DepthStencilView*     g_pDepthStencilView = nullptr;
ID3D11VertexShader*         g_pVertexShader = nullptr;
ID3D11PixelShader*          g_pPixelShader = nullptr;
ID3D11PixelShader*          g_pPixelShaderSolid = nullptr;
// Vetex and Indices
ID3D11InputLayout*          g_pVertexLayout = nullptr;
ID3D11Buffer*               g_pVertexBuffer = nullptr;
ID3D11Buffer*               g_pIndexBuffer = nullptr;
// Constant Buffer
ID3D11Buffer*               g_pConstantBuffer = nullptr;
ID3D11Buffer*               g_pCBNeverChanges = nullptr;
ID3D11Buffer*               g_pCBChangeOnResize = nullptr;
ID3D11Buffer*               g_pCBChangesEveryFrame = nullptr;
// Texture
ID3D11ShaderResourceView*   g_pTextureRV = nullptr;
ID3D11SamplerState*         g_pSamplerLinear = nullptr;


// Transform Matrices
Matrix                      g_WorldMat;
Matrix                      g_WorldMat2;
Matrix                      g_ViewMat;
Matrix                      g_ProjectionMat;
FLOAT4                      g_vMeshColor(0.7f, 0.7, 0.7f, 1.0f);

// 전역 프로퍼티
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름

// 전방 선언
ATOM                MyRegisterClass(HINSTANCE hInstance);
bool                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// D3D
bool                InitDevice();
void                CleanupDevice();

// Helper
bool                CompileShaderFromFile(const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

void                RenderCube();
void                RenderCube_Norm();
void                RenderCube_Tex();

bool                SetTriangle();
bool                SetCube();
bool                SetCubeWithNdotL();
bool                SetCubeWithTex();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);

    // 윈도우의 스타일을 지정하고,
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // DirectX 디바이스 초기화를 한다.
    if (!InitDevice()) {
        return FALSE;
    }

    // #1 삼각형 그릴 준비
#if 0
    if (!SetTriangle()) {
        return FALSE;
    }
#endif 
    // _mm_shuffle_ps , mm_mul_ps , _mm_add_ps 테스트
#if 0 
    Vector4 v1 = Vector4(1.f, 2.f, 3.f, 4.f);
    Vector4 v2 = Vector4(5.f, 6.f, 7.f, 8.f);

    Vector4 temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(3,2,1,0));
    //1278
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(0,1,2,3));
    //4365
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(0, 0, 0, 0));
    // 1155
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(1, 1, 1, 1));
    // 2266
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(2, 2, 2, 2));
    //3377
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(3, 3, 3, 3));
    //4488
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(0, 1, 0, 1));
    // 2165
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(1, 0, 1, 0));
    // 1256
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(0, 2, 0, 2));
    //3175
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(2, 0, 2, 0));
    //1357
    temp = v1;
    temp.m = _mm_shuffle_ps(temp.m, v2.m, _MM_SHUFFLE(0, 2, 0, 2));
    //3175

#endif
    // 매트릭스 테스트
#if 0 
    Matrix imat = MatrixIdentity();
    Matrix matTranspose = MatrixTranspose(imat);
    Matrix rotatematy = MatrixRotationX(45);
    rotatematy = MatrixRotationY(45);
    rotatematy = MatrixRotationZ(45);
    
#endif
    // XM 테스트
#if 0 
    XMMATRIX xMat(1.f, 2.f, 3.f, 4.f,
        5.f, 6.f, 7.f, 8.f,
        9.f, 10.f, 11.f, 12.f,
        13.f, 14.f, 15.f, 16.f);
    XMFLOAT4 xFloat(-1.f, -2.f, -3.f, -4.f);
    XMVECTOR xVec = XMLoadFloat4(&xFloat);

    xVec = XMVector3Transform(xVec, xMat);
    xVec = xVec;

    Matrix mat(1.f, 2.f, 3.f, 4.f,
        5.f, 6.f, 7.f, 8.f,
        9.f, 10.f, 11.f, 12.f,
        13.f, 14.f, 15.f, 16.f);

    Vector4 vec(-1.f, -2.f, -3.f, -4.f);

    vec = VectorTransform(vec, mat);
    vec = vec;
#endif
    // 큐브 그릴 준비
#if 0 
    if (!SetCube()) {
        return FALSE;
    }
#endif
    // 큐브 + NdotL Light
#if 0
    if (!SetCubeWithNdotL()) {
        return FALSE;
    }
#endif
#if 1
    if (!SetCubeWithTex()) {
        return FALSE;
    }
#endif 0  

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));

    MSG msg = { 0 };

    // 기본 메시지 루프입니다:
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            //RenderCube();
            //RenderCube_Norm();
            RenderCube_Tex();
        }
    }

    CleanupDevice();

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // 윈도우 사이즈를 정하고
   RECT rc = { 0, 0, 640, 480 };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, true);

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return false;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return true;
}

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

    for (UINT i = 0; i < numDriverTypes; i++) {
        g_driverType = driverTypes[i];
        // 이 커다란 함수 하나에서 드라이버 타입과 피쳐레밸을 확인하면서
        // 프로퍼티로 스왑체인, 디바이스, 피쳐레벨, 디바이스 컨텍스트를 생성한다.
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr, g_driverType, nullptr, createDeviceFlags,
            featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
            &sd, &g_pSwapChain,&g_pd3dDevice, &g_featureLevel, &g_pImmediateContext
        ); // 원래는 디바이스 생성과 스왑 체인 생성이 따로 이뤄지고, 그것을 나중에 결합하게 된다.
        if (SUCCEEDED(hr)) {
            break;
        }
    }
    if (FAILED(hr)) {
        return false;
    }
    
    // 렌더 타겟 뷰, 쉐이더 리소스 뷰 (읽기 전용)
    // 이런 뷰 들은 리소스를 사용하기 위한 파생 인터페이스로
    // 만들어진 친구이다. (근원은 텍스쳐이다. 텍스쳐는 쓰기가 가능하다.)
    ID3D11Texture2D* pBackBuffer = nullptr;
    // 스왑체인을 생성하면 기본적으로 나오는 텍스쳐를 백으로 준다. (프론드는 Desktop Window Manager에서 관리하는 애가 프론트이다.)
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (FAILED(hr)) {
        return false;
    }

    hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) {
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
    hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
    if (FAILED(hr)) {
        return false;
    }

    // depth stencil view를 만든다.
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSv;
    memset(&descDSv, 0, sizeof(descDSv));
    descDSv.Format = descDepth.Format;
    descDSv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSv.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSv, &g_pDepthStencilView);
    if (FAILED(hr)) {
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

void RenderCube()
{
#if 0
    // 렌더 타겟 뷰를 디바이스 컨텍스트에서 특정 색으로 싹 깔끔하게 해준다.
    float ClearColor[4] = { 0.f, 0.125f, 0.3f, 1.f };
    g_pd3dDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    g_pd3dDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pd3dDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
    g_pd3dDeviceContext->Draw(3, 0);

    // 백 버퍼를 프론트 버퍼로 보여준다.
    g_pSwapChain->Present(0, 0);
#endif
    // 시간을 계속 쌓아주고
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // 첫번째 큐브는 그냥 누적된 시간에 맞춰서 Y축으러 자전만 시켜주고
    g_WorldMat = MatrixRotationY(t);

    // 두번째 큐브는 궤도를 따라 공전을 하면서, 동시에 자전도 시켜준다.
    Matrix mSpin = MatrixRotationZ(-t);
    Matrix mOrbit = MatrixRotationY(-t * 2.0f);
    Matrix mTranslate = MatrixTranslation(-4.f, 0.f, 0.f);
    Matrix mScale = MatrixScale(0.3f, 0.3f, 0.3f);

    // 행렬 곱 순서는 스케일 -> 자전 -> 공전 (이동 + 회전) 순서로 곱하면 된다.
    g_WorldMat2 = mScale * mSpin * mTranslate * mOrbit;


    // 백 버퍼를 깔끔하게 치워주고
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // 뎁스 버퍼도 1.0으로 깔끔하게 지워준다.
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

    // ================
    // == 첫번째 큐브 ==
    // ================

    // 쉐이더로 넘겨줄 값을 초기화하고, 
    ConstantBuffer cb;
    cb.mWorld = MatrixTranspose(g_WorldMat);
    cb.mView = MatrixTranspose(g_ViewMat);
    cb.mProjection = MatrixTranspose(g_ProjectionMat);

    // Constant 버퍼를 위한 리소스 메모리인 g_pConstantBuffer에다가 그것을 넘겨준다. (첫번째 큐브)
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    // 삼각형을 그린다.
    g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
    // 설정된 상수버퍼를 세팅한다. (슬롯 번호, 갯수, 포인터)
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
    g_pImmediateContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list

    // ================
    // == 두번째 큐브 ==
    // ================

    // 쉐이더로 넘겨줄 값을 초기화하고, 
    ConstantBuffer cb2;
    cb2.mWorld = MatrixTranspose(g_WorldMat2);
    cb2.mView = MatrixTranspose(g_ViewMat);
    cb2.mProjection = MatrixTranspose(g_ProjectionMat);

    // Constant 버퍼를 위한 리소스 메모리인 g_pConstantBuffer에다가 그것을 넘겨준다. (두번째 큐브)
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb2, 0, 0);

    // 똑같은 버텍스 버퍼를 사용한다.
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    // 다 그린거를 화면에 뿌린다.
    g_pSwapChain->Present(0, 0);
}

void RenderCube_Norm()
{
    // 시간 업데이트
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // 빙글빙글 도는 큐브
    g_WorldMat = MatrixRotationY(t);

    // lighting parameter 세팅
    FLOAT4 vLightDirections[2] = {
        FLOAT4(-0.577f, 0.577f, -0.577f, 1.0f), //  고정적으로 뒤에서 쏴주는 친구
        FLOAT4(0.f, 0.f, -1.f, 1.0f), // 주위를 빙글빙글 돌면서 쏴주는 친구
    };

    FLOAT4 vLightColors[2] = {
        FLOAT4(0.5f, 0.5f, 0.5f,1.0f), // 대충 회색
        FLOAT4(0.8f,0.2f,0.f,1.f), // 주황색
    };

    // 빨간 조명을 공전하게 한다.
    Matrix mRotate = MatrixRotationY(-2.f* t);
    Vector4 vLightDir(vLightDirections[1]);
    vLightDir = VectorTransform(vLightDir, mRotate);
    vLightDirections[1] = vLightDir.GetFloat4();

    // 백 버퍼 지우고
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // 뎁스 버퍼 지우고 (1.0이 최대로)
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // 매트릭스와 라이트 변수를 업데이트 해준다.
    ConstantBuffer_Norm cb1;
    cb1.mWorld = MatrixTranspose(g_WorldMat);
    cb1.mView = MatrixTranspose(g_ViewMat);
    cb1.mProjection = MatrixTranspose(g_ProjectionMat);
    cb1.vLightDir[0] = vLightDirections[0].Nomalize();
    cb1.vLightDir[1] = vLightDirections[1].Nomalize();
    cb1.vLightColor[0] = vLightColors[0];
    cb1.vLightColor[1] = vLightColors[1];
    cb1.vOutputColor = FLOAT4(0, 0, 0, 0);
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

    // 큐브 그리기
    g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    for (int m = 0; m < 2; m++)
    {
        Vector4 vLightDirVec = Vector4(vLightDirections[m]) * 5.0f;
        Matrix mLight = MatrixTranslation(vLightDirVec.GetFloat4());
        Matrix mLightScale = MatrixScale(0.2f, 0.2f, 0.2f);
        mLight = mLightScale * mLight;

        // Update the world variable to reflect the current light
        cb1.mWorld = MatrixTranspose(mLight);
        cb1.vOutputColor = vLightColors[m];
        g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

        g_pImmediateContext->PSSetShader(g_pPixelShaderSolid, NULL, 0);
        g_pImmediateContext->DrawIndexed(36, 0, 0);
    }

    // 백 버퍼 그리기
    g_pSwapChain->Present(0, 0);
}

void RenderCube_Tex()
{
    // Update our time
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // 자전 시키기
    g_WorldMat = MatrixRotationY(t);

    // 색 변환
    g_vMeshColor.x = (sinf(t * 1.f) + 1.f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.f) + 1.f) * 0.5f;
    g_vMeshColor.z = (sinf(t * 5.f) + 1.f) * 0.5f;

    // 백 버퍼 지우기
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // 뎁스 버퍼 지우기
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // 매 프레임 마다 변하는 constant buffer 업데이트 하기
    CBChangesEveryFrame cb;
    cb.mWorld = MatrixTranspose(g_WorldMat);
    cb.vMeshColor = g_vMeshColor;
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    // 이제 GPU로 하여금 셰이더에 적힌, 레지스터 번호로 Contant buffer가 어디로 들어가야 하는지 알려주고
    // 계산을 돌리고 인덱스를 따라 삼각형을 그리도록 시킨다.
    g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
    g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
    g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
    g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
    g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
    g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    // 버퍼를 화면에 쏜다.
    g_pSwapChain->Present(0, 0);
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
    if (FAILED(hr)) {
        if (pErrorBlob != NULL)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        if (pErrorBlob) 
            pErrorBlob->Release();
        return false;
    }
    if (pErrorBlob) {
        pErrorBlob->Release();
    }

    return true;
}

void CleanupDevice()
{
    // Context에서 연관성을 싹 없애준 다음에
    if (g_pImmediateContext) {
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

bool SetTriangle()
{
    HRESULT hr = S_OK;

    // 버텍스 쉐이더 컴파일
    ID3DBlob* pVSBlob = nullptr;
    bool Result = CompileShaderFromFile(L"Tutorial02.fx", "VS", "vs_4_0", &pVSBlob);
    if (!Result) {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이터 만들기
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr)) {
        pVSBlob->Release();
        return false;
    }

    // Vertex Input layer 정의
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    UINT numElements = ARRAYSIZE(layout);

    //  input layout 만들기
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();

    if (FAILED(hr))
        return false;

    // input layout 세팅하기
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial02.fx", "PS", "ps_4_0", &pPSBlob);
    if (!Result) {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이터 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    if (FAILED(hr)) {
        return false;
    }

    // 버텍스 버퍼 만들기
    SimpleVertex vertices[] =
    {
        {FLOAT3(0.f, 0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)},
        {FLOAT3(0.5f, -0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)},
        {FLOAT3(-0.5f, -0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)}
    };

    // 버텍스 버퍼 디스크라이브를 지정한다.
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    // 버텍스 버퍼의 데이터를 세팅한다.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));
    InitData.pSysMem = vertices;
    // 디바이스를 버퍼를 초기화하면서 만들어준다.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // 버텍스 버퍼를 세팅한다.
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    // 버텍스 버퍼를 Input - Assembly 상태로 만든다.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // primitive topology는 간단하게 리스트로 설정한다.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

bool SetCube()
{
    HRESULT hr = S_OK;
    // 버텍스 쉐이더 컴파일
    ID3DBlob* pVSBlob = NULL;
    bool Result = CompileShaderFromFile(L"Tutorial05.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    // 버텍스 쉐이터 만들기
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    // Vertex Input layer 정의
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // Sementic 뒤에 있는 숫자는 똑같은 Sementic Name을 쓰고 싶을때, 인덱스로 넣는 친구이다.
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    // 저기 COLOR 부분에 12가 뭐냐면, 이후에 넣어준 Vertex 데이터에
    // 점->색->점->색 이렇게 데이터가 들어가는데,
    // 점이 float 3개니까 12바이트가 지나가면 COLOR가 나온다는 뜻이다.
    UINT numElements = ARRAYSIZE(layout);

    //  input layout 만들기
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr))
        return false;

    // input layout 세팅하기
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial05.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 픽셀 쉐이더 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼 만들기
    SimpleVertex vertices[] =
    {
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
    };

    // 버텍스 버퍼 디스크라이브를 지정한다.
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    // 버텍스 버퍼의 데이터를 세팅한다.
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    // 여기가 포인터를 넣는 곳이다.
    InitData.pSysMem = vertices;


    // 디바이스를 버퍼를 초기화하면서 만들어준다.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼를 세팅한다.
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    // 버텍스 버퍼를 Input - Assembly 상태로 만든다.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Vertex를 여러번 쓰기 위한 Index 버퍼를 만든다.
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        0,5,4,
        1,5,0,

        3,4,7,
        0,4,3,

        1,6,5,
        2,6,1,

        2,7,6,
        3,7,2,

        6,4,5,
        7,4,6,
    };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;        // 12개의 삼각형
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
        return false;

    // 인덱스 버퍼 만들기
    // 변화할 여지가 있으면 원래는 Render에서 해준다.
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // primitive topology는 간단하게 리스트로 설정한다.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 셰이더에 들어가는 상수 버퍼 만들기
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
    if (FAILED(hr))
        return false;

    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 카메라 메트릭스 초기화
    Vector4 Eye = Vector4(0.0f, 1.0f, -5.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Perspective로 프로젝션 매트릭스 초기화
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv2, (float)width / (float)height, 0.01f, 100.0f);

    return true;
}

bool SetCubeWithNdotL()
{
    HRESULT hr = S_OK;
    // 버텍스 쉐이더 컴파일
    ID3DBlob* pVSBlob = NULL;
    bool Result = CompileShaderFromFile(L"Tutorial06.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이터 만들기
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // Vertex Input layer 정의
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // Sementic 뒤에 있는 숫자는 똑같은 Sementic Name을 쓰고 싶을때, 인덱스로 넣는 친구이다.
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layout);

    //  input layout 만들기
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr))
        return false;

    // input layout 세팅하기
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일 #1 (빛을 반사하는 친구)
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial06.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 픽셀 쉐이더 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // 픽셀 쉐이더 컴파일 #1 (빛을 반사하지 않는 친구)
    pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial06.fx", "PSSolid", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 픽셀 쉐이더 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShaderSolid);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼 만들기
    SimpleVertex_Norm vertices[] =
    {
         { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },

        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },

        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },

        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },

        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },

        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) }, // 이 점이 뭔가 이상하다.
    };

    // 버텍스 버퍼 디스크라이브를 지정한다.
    D3D11_BUFFER_DESC bd;
    memset(&bd ,0 , sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex_Norm) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    // 버텍스 버퍼의 데이터를 세팅한다.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));

    // 여기가 포인터를 넣는 곳이다.
    InitData.pSysMem = vertices;


    // 디바이스를 버퍼를 초기화하면서 만들어준다.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼를 세팅한다.
    UINT stride = sizeof(SimpleVertex_Norm);
    UINT offset = 0;

    // 버텍스 버퍼를 Input - Assembly 상태로 만든다.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Vertex를 여러번 쓰기 위한 Index 버퍼를 만든다.
    WORD indices[] =
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
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;        // 점을 많이 쓰는 이유는 한 평면에 일관적인 Normal을 주기 위함이다.
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
        return false;

    // 인덱스 버퍼 만들기
    // 변화할 여지가 있으면 원래는 Render에서 해준다.
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // primitive topology는 간단하게 리스트로 설정한다.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 셰이더에 들어가는 상수 버퍼 만들기
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer_Norm);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
    if (FAILED(hr))
        return false;

    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 카메라 메트릭스 초기화
    Vector4 Eye = Vector4(0.0f, 4.0f, -10.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Perspective로 프로젝션 매트릭스 초기화
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv4, (float)width / (float)height, 0.01f, 100.0f);

    return true;
}

bool SetCubeWithTex()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Shader를 컴파일 해서 Binary Large Object file로 만들기
    ID3DBlob* pVSBlob = nullptr;
    bool Result = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false) {
        MessageBox(nullptr,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이더 객체 만들기
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr)) {
        pVSBlob->Release();
        return false;
    }

    // input layout 정해주기
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numElements = ARRAYSIZE(layout);

    // input layout 만들기
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr)) {
        return false;
    }

    // input layer 세팅해주기
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일 해주기
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
    if (Result == false) {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 픽셀 쉐이더 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr)) {
        return false;
    }

    // 버텍스 버퍼 만들기
    SimpleVertex_Tex vertices[] =
    {
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },
    };

    // 버텍스 버퍼 속성 정해주기
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex_Tex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    // 버텍스 데이터 포인터를 연결해준다.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // 버텍스 버퍼 객체 세팅
    UINT stride = sizeof(SimpleVertex_Tex);
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // 인덱스 버퍼 만들기
    WORD indices[] =
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

    // 인덱스 버퍼 속성 정해주기
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    
    // 인덱스 데이터 포인터 연결해주기
    InitData.pSysMem = indices;

    // 인덱스 버퍼 만들어주기
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr)) {
        return false;
    }
        
    // 인덱스 버퍼 세팅
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // triangle list로 그리기
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Constant 버퍼 만들기
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    //  #1 
    bd.ByteWidth = sizeof(CBNeverChanges);
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBNeverChanges);
    if (FAILED(hr)) {
        return false;
    }
    // #2
    bd.ByteWidth = sizeof(CBChangeOnResize);
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangeOnResize);
    if (FAILED(hr)) {
        return false;
    }
    // #3
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangesEveryFrame);
    if (FAILED(hr)) {
        return false;
    }

    // Load the Texture
    hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, L"seafloor.dds", nullptr, nullptr, &g_pTextureRV, nullptr);
    if (FAILED(hr)) {
        return false;
    }

    // 텍스쳐의 Sampler state 만들기
    D3D11_SAMPLER_DESC sampDesc;
    memset(&sampDesc, 0, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
    if (FAILED(hr)) {
        return false;
    }

    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 뷰 매트릭스 초기화
    Vector4 Eye = Vector4(0.0f, 3.0f, -6.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    // 변할일이 없는 constant buffer 초기화
    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = MatrixTranspose(g_ViewMat);
    g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbNeverChanges, 0, 0);

    // 프로젝션 매트릭스 초기화
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv4, width / (float)height, 0.01, 100.f);

    // 윈도우 크기 변환 시에 변하는 Constant buffer 초기화
    CBChangeOnResize cbChangeOnResize;
    cbChangeOnResize.mProjection = MatrixTranspose(g_ProjectionMat);
    g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangeOnResize, 0, 0);

    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
