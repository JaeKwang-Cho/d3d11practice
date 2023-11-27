#pragma once

// Win32
extern HINSTANCE                   g_hInst;  // ���� �ν��Ͻ�
extern HWND                        g_hWnd;   // ������ �ڵ�
// d3d Attribute
extern D3D_DRIVER_TYPE             g_driverType; // ����̹� ����
extern D3D_FEATURE_LEVEL           g_featureLevel; // ����� directx ����(?)
// d3d Property
extern ID3D11Device* g_pd3dDevice; // ���ҽ��� �����, ���÷��� ��� ����� ����ϰ� �ϴ� ��
extern ID3D11DeviceContext* g_pImmediateContext; // ����̽��� �Ͽ��� ���������� ���¸� �����Ͽ�, ������ ����� �����µ� ����Ѵ�.
extern IDXGISwapChain* g_pSwapChain; // ���� ���۸��� ���� Surface(IDXGISurface)�� �����ϴµ� ����Ѵ�.
// Buffers
extern ID3D11RenderTargetView* g_pRenderTargetView;
extern ID3D11Texture2D* g_pDepthStencil;
extern ID3D11DepthStencilView* g_pDepthStencilView;

// Temp Matrix
extern Matrix                      g_WorldMat;
extern Matrix                      g_WorldMat2;
extern Matrix                      g_ViewMat;
extern Matrix                      g_ProjectionMat;
extern FLOAT4                      g_vMeshColor;

extern ID3D11Buffer*				g_pConstantBuffer;
extern ID3D11Buffer*				g_pCBNeverChanges;
extern ID3D11Buffer*				g_pCBChangeOnResize;
extern ID3D11Buffer*				g_pCBChangesEveryFrame;

class CCore
{
public:
	SINGLETON(CCore)

private:
	HWND m_hWnd;
	POINT m_ptResolution;

public:
	int Init(HWND _hWnd, POINT _ptResolution);

	void Progress();
	void CheckMessage(_In_ const MSG* lpMsg);

private:
	void update();
	void render();

public:
	HWND GetMainHwnd()
	{
		return m_hWnd;
	}

};
