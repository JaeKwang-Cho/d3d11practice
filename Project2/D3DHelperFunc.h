#pragma once

// =================
// ====전역  변수====
// =================
// Win32
extern HINSTANCE                   g_hInst;  // 현재 인스턴스
extern HWND                        g_hWnd;   // 윈도우 핸들
// d3d Attribute
extern D3D_DRIVER_TYPE             g_driverType;  // 드라이버 종류
extern D3D_FEATURE_LEVEL           g_featureLevel; // 사용할 directx 버전(?)
// d3d Property
extern ID3D11Device*				g_pd3dDevice; // 리소스를 만들고, 디스플레이 어뎁터 기능을 사용하게 하는 것
extern ID3D11DeviceContext*			g_pImmediateContext; // 디바이스로 하여금 파이프라인 상태를 제어하여, 렌더링 명령을 내리는데 사용한다.
extern IDXGISwapChain*				g_pSwapChain; // 더블 버퍼링을 위한 Surface(IDXGISurface)를 구성하는데 사용한다.
extern ID3D11RenderTargetView*		g_pRenderTargetView;
// Shader and Property
extern ID3D11Texture2D*				g_pDepthStencil;
extern ID3D11DepthStencilView*		g_pDepthStencilView;


extern ID3D11VertexShader* g_pVertexShader;
extern ID3D11PixelShader* g_pPixelShader;
extern ID3D11PixelShader* g_pPixelShaderSolid;
// Vetex and Indices
extern ID3D11InputLayout* g_pVertexLayout;
extern ID3D11Buffer* g_pVertexBuffer;
extern ID3D11Buffer* g_pIndexBuffer;
// Constant Buffer
extern ID3D11Buffer* g_pConstantBuffer;
extern ID3D11Buffer* g_pCBNeverChanges;
extern ID3D11Buffer* g_pCBChangeOnResize;
extern ID3D11Buffer* g_pCBChangesEveryFrame;
// Texture
extern ID3D11ShaderResourceView* g_pTextureRV;
extern ID3D11SamplerState* g_pSamplerLinear;


// D3D
bool                InitDevice();
void                CleanupDevice();

// Helper
bool                CompileShaderFromFile(const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);