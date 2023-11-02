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