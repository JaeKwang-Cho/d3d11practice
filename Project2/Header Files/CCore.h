#pragma once

// Temp Matrix
extern Matrix                      g_WorldMat;
extern Matrix                      g_WorldMat2;
extern Matrix                      g_ViewMat;
extern Matrix                      g_ProjectionMat;
extern Matrix					   g_OrthogratphicMat;
extern FLOAT4                      g_vMeshColor;

extern ID3D11Buffer*				g_pConstantBuffer;
extern ID3D11Buffer*				g_pCBMVPMat;
extern ID3D11Buffer*				g_pCBLightBuffer;

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
