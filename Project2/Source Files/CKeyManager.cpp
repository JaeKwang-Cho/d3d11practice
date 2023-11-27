#include "pch.h"
#include "CKeyManager.h"
#include "CMouseManager.h"

// �̷��� ���� 
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

// �굵 Core���� ��� ������ ���鼭 key ���¸� üũ�� ���̴�.
void CKeyManager::Update()
{
	// �����찡 ������ �Ǿ��ִ���, ��Ŀ���� �Ǿ��ִ��� Ȯ���Ѵ�.

	// �� �Ŵ����� ������ �ִ� ������ �ڵ��� �����ͼ�, ���� ������ �������� Ȯ�� �� �� �� �ִ�.
	//HWND hMainWnd = CCore::GetInstance()->GetMainHwnd();
	// ���� ��Ŀ�̵� ������ �ڵ��� �����´�.
	HWND hWnd = GetFocus();
	// nullptr�̶��, ���� app�� �����찡 ��Ŀ�� �Ǿ� ���� �ʴٴ� ���̴�.

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::last; i++)
		{
			// �� �Լ��� ����ϱ� ���ؼ� + �ε����� ���߱� ���ؼ� ���� �迭�� ���� ����Ѵ�.
			if (GetAsyncKeyState(g_VirtualKeyArr[i]) & 0x8000)
			{
				// ���� �����Ӱ� ���� �������� ���¸� ���Ͽ���
				// ���� KEY_STATE�� �����Ѵ�.
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
	// ������ ��Ŀ�� ���� ����
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