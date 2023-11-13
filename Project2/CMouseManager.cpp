#include "pch.h"
#include "CMouseManager.h"
#include "CKeyManager.h"

void CMouseManager::Init()
{
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = CCore::GetInstance()->GetMainHwnd();
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
}

void CMouseManager::Update()
{
	// Mouse 위치 계산
	// Window 제공 함수를 사용하면 된다.

	POINT ptPos = {};
	// 윈도우 기준이 아니라, 모니터 전체 기준이다.
	GetCursorPos(&ptPos);
	// 그래서 이것을 윈도우 기준으로 바꿔줘야 한다.
	ScreenToClient(CCore::GetInstance()->GetMainHwnd(), &ptPos);

	m_vMousePos = FLOAT2(ptPos);
}

void CMouseManager::Render(bool _bRelative)
{
	wchar_t szBuffer[255] = {};

	if (_bRelative)
	{
		swprintf_s(szBuffer, L"MousePos (x, y) = (%f, %f)", m_vRelativeMouseMov.u, m_vRelativeMouseMov.v);
	}
	else
	{
		swprintf_s(szBuffer, L"MousePos (x, y) = (%f, %f)", m_vMousePos.u, m_vMousePos.v);		
	}
	SetWindowText(CCore::GetInstance()->GetMainHwnd(), szBuffer);
}

void CMouseManager::MouseMoved(_In_ const BYTE* lpb)
{
	RAWINPUT* raw = (RAWINPUT*)lpb;
	int xPosRelative = raw->data.mouse.lLastX;
	int yPosRelative = raw->data.mouse.lLastY;

	m_vRelativeMouseMov = FLOAT2((float)xPosRelative, (float)yPosRelative);
}

void CMouseManager::RezeroRelativePos()
{
	m_vRelativeMouseMov = FLOAT2(0.f, 0.f);
}

void CMouseManager::SetCursorMiddle()
{
	RECT rc = {};
	GetClientRect(CCore::GetInstance()->GetMainHwnd(), &rc);

	POINT ptPos = { (rc.right - rc.left) / 2, (rc.bottom - rc.top) / 2 };
	ScreenToClient(CCore::GetInstance()->GetMainHwnd(), &ptPos);

	SetCursorPos(ptPos.x, ptPos.y);
}

CMouseManager::CMouseManager()
	: m_vMousePos(0.f,0.f)
	, m_vRelativeMouseMov(0.f,0.f)
{

}

CMouseManager::~CMouseManager()
{

}
