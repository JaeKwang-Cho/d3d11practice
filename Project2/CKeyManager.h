#pragma once

enum class KEY {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q, W, E, R,
	T, Y, U, I,
	O, P, A, S,
	D, F, G, H,
	J, K, L, Z,
	X, C, V, B,
	N, M,
	ALT,
	CTRL,
	LSHIFT,
	ENTER,
	ESC,
	SPACE,

	LMOUSE,
	RMOUSE,

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
