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
	// Mouse ��ġ ���
	// Window ���� �Լ��� ����ϸ� �ȴ�.

	POINT ptPos = {};
	// ������ ������ �ƴ϶�, ����� ��ü �����̴�.
	GetCursorPos(&ptPos);
	// �׷��� �̰��� ������ �������� �ٲ���� �Ѵ�.
	ScreenToClient(CCore::GetInstance()->GetMainHwnd(), &ptPos);

	m_vMousePos = FLOAT2(ptPos);
}

void CMouseManager::Render()
{
	wchar_t szBuffer[255] = {};
	swprintf_s(szBuffer, L"MousePos (x, y) = (%f, %f)", m_vMousePos.u, m_vMousePos.v);
	SetWindowText(CCore::GetInstance()->GetMainHwnd(), szBuffer);
}

void CMouseManager::MouseMoved(_In_ const BYTE* lpb)
{
	RAWINPUT* raw = (RAWINPUT*)lpb;
	int xPosRelative = raw->data.mouse.lLastX;
	int yPosRelative = raw->data.mouse.lLastY;

	m_vRelativeMouseMov = FLOAT2((float)xPosRelative, (float)yPosRelative);

	wchar_t szBuffer[255] = {};
	swprintf_s(szBuffer, L"MousePos (x, y) = (%d, %d)", xPosRelative, yPosRelative);
	SetWindowText(CCore::GetInstance()->GetMainHwnd(), szBuffer);
}

CMouseManager::CMouseManager()
	: m_vMousePos(0.f,0.f)
	, m_vRelativeMouseMov(0.f,0.f)
{

}

CMouseManager::~CMouseManager()
{

}