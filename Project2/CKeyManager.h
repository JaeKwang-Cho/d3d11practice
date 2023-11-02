#pragma once

enum class KEY {
	K_LEFT,
	K_RIGHT,
	K_UP,
	K_DOWN,
	K_Q, K_W, K_E, K_R,
	K_T, K_Y, K_U, K_I,
	K_O, K_P, K_A, K_S,
	K_D, K_F, K_G, K_H,
	K_J, K_K, K_L, K_Z,
	K_X, K_C, K_V, K_B,
	K_N, K_M,
	K_ALT,
	K_CTRL,
	K_LSHIFT,
	K_ENTER,
	K_ESC,
	K_SPACE,

	K_LMOUSE,
	K_RMOUSE,

	last
};

enum class KEY_STATE {
	KS_NONE,	// �������� ������ �ʾҰ�, ���ݵ� ������ ���� ����
	KS_TAP,	// �� ���� ����
	KS_HOLD,	// ������ �ִ�
	KS_AWAY,	// �� �� ����
};

struct tKeyInfo {
	KEY_STATE	eState; // Ű�� ���� ��
	bool		bPrevPush;	// ������ �������� �־�����
};

class CKeyManager
{
	SINGLETON(CKeyManager)

private:
	vector<tKeyInfo> m_VecKey;
	FLOAT2 m_vMousePos;

public:
	void Init();
	void Update();
	KEY_STATE GetKeyState(KEY _key)
	{
		return m_VecKey[(int)_key].eState;
	}

	FLOAT2 GetMousePos()
	{
		return m_vMousePos;
	}
};
