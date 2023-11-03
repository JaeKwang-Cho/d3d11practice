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
	KS_NONE,	// 이전에도 눌리지 않았고, 지금도 눌리지 않은 상태
	KS_TAP,	// 막 누른 시점
	KS_HOLD,	// 누르고 있는
	KS_AWAY,	// 막 뗀 시점
};

struct tKeyInfo {
	KEY_STATE	eState; // 키의 상태 값
	bool		bPrevPush;	// 이전에 눌린적이 있었는지
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
