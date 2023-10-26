// project1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <d3d11.h>
#include <d3dx11.h>

#include "framework.h"
#include "main.h"
#include "pch.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE                   g_hInst = nullptr;  // 현재 인스턴스
HWND                        g_hWnd = nullptr;   // 윈도우 핸들
D3D_DRIVER_TYPE             g_driverType = D3D_DRIVER_TYPE_NULL; // 드라이버 종류
D3D_FEATURE_LEVEL           g_featureLevel = D3D_FEATURE_LEVEL_11_1; // 사용할 directx 버전(?)
ID3D11Device*               g_pd3Device = nullptr; // 리소스를 만들고, 디스플레이 어뎁터 기능을 사용하게 하는 것
ID3D11DeviceContext*        g_pd3DeviceContext = nullptr; // 디바이스로 하여금 파이프라인 상태를 제어하여, 렌더링 명령을 내리는데 사용한다.
IDXGISwapChain*             g_pSwapChain = nullptr; // 더블 버퍼링을 위한 Surface(IDXGISurface)를 구성하는데 사용한다.
ID3D11RenderTargetView*     g_pRenderTargetView = nullptr;

// 전역 프로퍼티
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름

// 전방 선언
ATOM                MyRegisterClass(HINSTANCE hInstance);
bool                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool                InitDevice();
void                CleanupDevice();
void                Render();


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

    // 스왑 체인 프로퍼티를 설정한다.
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
            &sd, &g_pSwapChain,&g_pd3Device, &g_featureLevel, &g_pd3DeviceContext
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

    hr = g_pd3Device->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr)) {
        return false;
    }

    // 렌더타겟과 깊이 스텐실 버퍼를 Output-Merge 상태로 바인딩 한다.
    g_pd3DeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

    // 뷰포트를 설정한다.
    D3D11_VIEWPORT vp;
    vp.Width = (float)width;
    vp.Height = (float)height;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    // 뷰포트 구조체를 레스터라이즈 단계에 바인딩 한다.
    g_pd3DeviceContext->RSSetViewports(1, &vp);

    return true;
}

void Render()
{
    float ClearColor[4] = { 0.f, 0.125f, 0.3f, 1.f };
    // 렌더 타겟 뷰를 디바이스 컨텍스트에서 특정 색으로 싹 깔끔하게 해준다.
    g_pd3DeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
    g_pSwapChain->Present(0, 0);
}


void CleanupDevice()
{
    // Context에서 연관성을 싹 없애준 다음에
    if (g_pd3DeviceContext) {
        g_pd3DeviceContext->ClearState();
    }
    // 얘네는 다 COM 객체 들이여서 이렇게 해제를 해줘야 한다.
    if(g_pRenderTargetView){
        g_pRenderTargetView->Release();
    }

    if (g_pSwapChain) {
        g_pSwapChain->Release();
    }

    if (g_pd3DeviceContext) {
        g_pd3DeviceContext->Release();
    }

    if (g_pd3Device) {
        g_pd3Device->Release();
    }
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
