#include "pch.h"
#include "CCore.h"

#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CMouseManager.h"
#include "CSceneManager.h"
#include "CRenderManager.h"

int CCore::Init(HWND _hWnd, POINT _ptResolution)
{
	// 윈도우 기본 정보 받기
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 해상도에 맞게 윈도우 수정
	{
		RECT rt = { 0, 0, m_ptResolution.x , m_ptResolution.y };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
		SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
	}

	//Manager 초기화
	{
		CTimeManager::GetInstance()->Init();
		CKeyManager::GetInstance()->Init();
		CMouseManager::GetInstance()->Init();
		CSceneManager::GetInstance()->Init();
		CRenderManager::GetInstance()->Init();
	}

	return S_OK;
}

void CCore::Progress()
{
	update();

	render();

	return;
}

void CCore::CheckMessage(_In_ const MSG* lpMsg)
{
	UINT message = lpMsg->message;
	WPARAM wParam = lpMsg->wParam;
	LPARAM lParam = lpMsg->lParam;

	switch (message)
	{
	case WM_INPUT:
	{
		UINT dwSize = sizeof(RAWINPUT);
		static BYTE lpb[sizeof(RAWINPUT)];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;
		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			CMouseManager::GetInstance()->MouseMoved(lpb);
		}
	}
	break;
	default:
	{
	}
	break;
	}
}

void CCore::update()
{
	// ===========
	// ===Update==
	// ===========
	// 메니저 업데이트 루프
	
	CTimeManager::GetInstance()->Update();
	CKeyManager::GetInstance()->Update();
	CMouseManager::GetInstance()->Update();
	CSceneManager::GetInstance()->Update();
	
}

void CCore::render()
{
	// 최소화시 렌더링 안하기
	if (g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
	{
		return;
	}
	CTimeManager::GetInstance()->Render();

	// ===================
	// ==Rendering==
	// ===================

	CSceneManager::GetInstance()->Render();
	CRenderManager::GetInstance()->Render();
}

CCore::CCore() :
	m_hWnd(0), m_ptResolution{}
{

}

CCore::~CCore()
{
}