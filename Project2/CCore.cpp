#include "pch.h"
#include "CCore.h"

#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"

int CCore::Init(HWND _hWnd, POINT _ptResolution)
{
	// ������ �⺻ ���� �ޱ�
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// �ػ󵵿� �°� ������ ����
	{
		RECT rt = { 0, 0, m_ptResolution.x , m_ptResolution.y };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
		SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
	}

	//Manager �ʱ�ȭ
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
	// �޴��� ������Ʈ ����
	{
		CTimeManager::GetInstance()->Update();
		CKeyManager::GetInstance()->Update();
	}
	// �� ������Ʈ ����
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