// project1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

#include <xnamath.h>

#include "framework.h"
#include "main.h"
#include "pch.h"

#define MAX_LOADSTRING (100)
#define SHADER_BUFFER_SIZE (2048)

// =================
// ====전역  변수====
// =================
// Win32
HINSTANCE                   g_hInst = nullptr;  // 현재 인스턴스
HWND                        g_hWnd = nullptr;   // 윈도우 핸들
// d3d
D3D_DRIVER_TYPE             g_driverType = D3D_DRIVER_TYPE_NULL; // 드라이버 종류
D3D_FEATURE_LEVEL           g_featureLevel = D3D_FEATURE_LEVEL_11_1; // 사용할 directx 버전(?)
ID3D11Device*               g_pd3dDevice = nullptr; // 리소스를 만들고, 디스플레이 어뎁터 기능을 사용하게 하는 것
ID3D11DeviceContext*        g_pd3dDeviceContext = nullptr; // 디바이스로 하여금 파이프라인 상태를 제어하여, 렌더링 명령을 내리는데 사용한다.
IDXGISwapChain*             g_pSwapChain = nullptr; // 더블 버퍼링을 위한 Surface(IDXGISurface)를 구성하는데 사용한다.
ID3D11RenderTargetView*     g_pRenderTargetView = nullptr;
// Shader and Property
ID3D11VertexShader*         g_pVertexShader = nullptr;
ID3D11PixelShader*          g_pPixelShader = nullptr;
// Vetex and Indices
ID3D11InputLayout* g_pVertexLayout = nullptr;
ID3D11Buffer* g_pVertexBuffer = nullptr;
ID3D11Buffer* g_pIndexBuffer = NULL;
ID3D11Buffer* g_pConstantBuffer = NULL;

// Transform Matrices
Matrix                      g_WorldMat;
Matrix                      g_ViewMat;
Matrix                      g_ProjectionMat;

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

void                Render();
bool                SetTriangle();
bool                SetCube();


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

    
#if 1 // 삼각형 그릴 준비
    if (!SetTriangle()) {
        return FALSE;
    }
#endif 
#if 0 // _mm_shuffle_ps 테스트
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

#if 0 // 매트릭스 테스트
    Matrix imat = MatrixIdentity();
    Matrix matTranspose = MatrixTranspose(imat);
    Matrix rotatematy = MatrixRotationX(45);
    rotatematy = MatrixRotationY(45);
    rotatematy = MatrixRotationZ(45);
    
#endif
#if 1 // 큐브 그릴 준비
    if (!SetCube()) {
        return FALSE;
    }
#endif
    


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
            Render();
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
            &sd, &g_pSwapChain,&g_pd3dDevice, &g_featureLevel, &g_pd3dDeviceContext
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

    // 렌더타겟과 깊이 스텐실 버퍼를 Output-Merge 상태로 바인딩 한다.
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

    // 뷰포트를 설정한다.
    D3D11_VIEWPORT vp;
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    // 뷰포트 구조체를 레스터라이즈 단계에 바인딩 한다.
    g_pd3dDeviceContext->RSSetViewports(1, &vp);

    return true;
}

void Render()
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
    // Update our time
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

    //
    // Animate the cube
    //
    g_WorldMat = MatrixRotationY(t);

    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    g_pd3dDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    //
    // Update variables
    //
    ConstantBuffer cb;
    cb.mWorld = MatrixTranspose(g_WorldMat);
    cb.mView = MatrixTranspose(g_ViewMat);
    cb.mProjection = MatrixTranspose(g_ProjectionMat);
    g_pd3dDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

    //
    // Renders a triangle
    //
    g_pd3dDeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
    g_pd3dDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pd3dDeviceContext->PSSetShader(g_pPixelShader, NULL, 0);
    g_pd3dDeviceContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list

    //
    // Present our back buffer to our front buffer
    //
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
    if (g_pd3dDeviceContext) {
        g_pd3dDeviceContext->ClearState();
    }

    // 얘네는 다 COM 객체 들이여서 이렇게 해제를 해줘야 한다.
    // 해제하는 순서는 잘 모르겠다.
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pVertexLayout) g_pVertexLayout->Release();
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pPixelShader) g_pPixelShader->Release();
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pd3dDeviceContext) g_pd3dDeviceContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();
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
    g_pd3dDeviceContext->IASetInputLayout(g_pVertexLayout);

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
    g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // primitive topology는 간단하게 리스트로 설정한다.
    g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

bool SetCube()
{
    HRESULT hr = S_OK;
    // 버텍스 쉐이더 컴파일
    ID3DBlob* pVSBlob = NULL;
    bool Result = CompileShaderFromFile(L"Tutorial04.fx", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr))
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
    g_pd3dDeviceContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial04.fx", "PS", "ps_4_0", &pPSBlob);
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
    InitData.pSysMem = vertices;
    // 디바이스를 버퍼를 초기화하면서 만들어준다.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼를 세팅한다.
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    // 버텍스 버퍼를 Input - Assembly 상태로 만든다.
    g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

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
    g_pd3dDeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // primitive topology는 간단하게 리스트로 설정한다.
    g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
    Vector4 At = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
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
