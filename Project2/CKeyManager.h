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
