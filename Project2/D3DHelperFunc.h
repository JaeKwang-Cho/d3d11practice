#pragma once

// =================
// ====����  ����====
// =================
// Win32
extern HINSTANCE                   g_hInst;  // ���� �ν��Ͻ�
extern HWND                        g_hWnd;   // ������ �ڵ�
// d3d Attribute
extern D3D_DRIVER_TYPE             g_driverType;  // ����̹� ����
extern D3D_FEATURE_LEVEL           g_featureLevel; // ����� directx ����(?)
// d3d Property
extern ID3D11Device*				g_pd3dDevice; // ���ҽ��� �����, ���÷��� ��� ����� ����ϰ� �ϴ� ��
extern ID3D11DeviceContext*			g_pImmediateContext; // ����̽��� �Ͽ��� ���������� ���¸� �����Ͽ�, ������ ����� �����µ� ����Ѵ�.
extern IDXGISwapChain*				g_pSwapChain; // ���� ���۸��� ���� Surface(IDXGISurface)�� �����ϴµ� ����Ѵ�.

// states
extern ID3D11RasterizerState*		g_pRasterizerState;
extern ID3D11BlendState*			g_pBlendState;
extern ID3D11DepthStencilState*		g_pDepthStencilState;

extern ID3D11RenderTargetView*		g_pRenderTargetView;
// Shader and Property
extern ID3D11Texture2D*				g_pDepthStencil;
extern ID3D11DepthStencilView*		g_pDepthStencilView;

// D3D
bool                InitDevice();
void                CleanupDevice();
void				CleanupSamples();
HRESULT				ResizeWindow();// �ڵ忡�� ������ ����� �ٲٰ� ������ ȣ���ϴ� ��
void				CallbackResizeWindow();

// Helper
bool                CompileShaderFromFile(const wchar_t* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);