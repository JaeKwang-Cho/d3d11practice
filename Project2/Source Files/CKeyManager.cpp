#include "pch.h"
#include "CKeyManager.h"
#include "CMouseManager.h"

// 이렇게 실제 
int g_VirtualKeyArr[(int)KEY::last] = {

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	'Q', 'W', 'E', 'R',
	'T', 'Y', 'U', 'I',
	'O', 'P', 'A', 'S',
	'D', 'F', 'G', 'H',
	'J', 'K', 'L', 'Z',
	'X', 'C', 'V', 'B',
	'N', 'M',
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_RETURN,
	VK_ESCAPE,
	VK_SPACE,
	VK_LBUTTON,
	VK_RBUTTON,

};

CKeyManager::CKeyManager()
{


}

CKeyManager::~CKeyManager()
{

}

void CKeyManager::Init()
{
	for (int i = 0; i < (int)KEY::last; i++)
	{
		m_VecKey.push_back(tKeyInfo{ KEY_STATE::KS_NONE, false });
	}
}

// 얘도 Core에서 계속 루프를 돌면서 key 상태를 체크할 것이다.
void CKeyManager::Update()
{
	// 윈도우가 선택이 되어있는제, 포커싱이 되어있는지 확인한다.

	// 이 매니저를 가지고 있는 윈도우 핸들을 가져와서, 메인 윈도우 선택인지 확인 할 수 도 있다.
	//HWND hMainWnd = CCore::GetInstance()->GetMainHwnd();
	// 현재 포커싱된 윈도우 핸들을 가져온다.
	HWND hWnd = GetFocus();
	// nullptr이라면, 현재 app의 윈도우가 포커싱 되어 있지 않다는 것이다.

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::last; i++)
		{
			// 이 함수를 사용하기 위해서 + 인덱스를 맞추기 위해서 전역 배열을 만들어서 사용한다.
			if (GetAsyncKeyState(g_VirtualKeyArr[i]) & 0x8000)
			{
				// 현제 프레임과 이전 프레임의 상태를 비교하여서
				// 현재 KEY_STATE를 설정한다.
				if (m_VecKey[i].bPrevPush)
				{
					m_VecKey[i].eState = KEY_STATE::KS_HOLD;
				}
				else
				{
					m_VecKey[i].eState = KEY_STATE::KS_TAP;
				}
				m_VecKey[i].bPrevPush = true;
			}
			else
			{
				if (m_VecKey[i].bPrevPush)
				{
					m_VecKey[i].eState = KEY_STATE::KS_AWAY;
				}
				else
				{
					m_VecKey[i].eState = KEY_STATE::KS_NONE;
				}
				m_VecKey[i].bPrevPush = false;
			}
		}
	}
	// 윈도우 포커싱 해제 상태
	else
	{
		for (int i = 0; i < (int)KEY::last; i++)
		{
			m_VecKey[i].bPrevPush = false;

			if (KEY_STATE::KS_TAP == m_VecKey[i].eState || KEY_STATE::KS_HOLD == m_VecKey[i].eState)
			{
				m_VecKey[i].eState = KEY_STATE::KS_AWAY;
			}
			else if (m_VecKey[i].eState == KEY_STATE::KS_AWAY)
			{
				m_VecKey[i].eState = KEY_STATE::KS_NONE;
			}
		}
	}
}