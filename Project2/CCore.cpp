#include "pch.h"
#include "CCore.h"

#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"

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
		CSceneManager::GetInstance()->Init();
	}

	return S_OK;
}

void CCore::Progress()
{
	// ===========
	// ===Update==
	// ===========
	// 메니저 업데이트 루프
	{
		CTimeManager::GetInstance()->Update();
		CKeyManager::GetInstance()->Update();
	}
	// 씬 업데이트 루프
	{
		CTimeManager::GetInstance()->Render();
		CSceneManager::GetInstance()->Render();
	}

	// =============
	// ==Rendering==
	// =============
	{

	}

	return;
}

void CCore::update()
{
}

void CCore::render()
{
}

CCore::CCore() :
	m_hWnd(0), m_ptResolution{}
{

}

CCore::~CCore()
{
}