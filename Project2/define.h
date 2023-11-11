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


enum class GROUP_TYPE { // Scene 안에 Object들을 나누는 그룹이다.
	Default,

	END = 32,
};

enum class SCENE_TYPE {
	Default,

	END,
};

enum class ROT_ORDER {
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};

#define PI  3.141592654f
#define PIMul2  6.283185307f
#define PIDiv2 1.570796327f
#define PIDiv4 0.785398163f
#define OneDivPI 0.318309886f
#define OneDiv2PI 0.159154943f

#define MAX_LOADSTRING (100)
#define SHADER_BUFFER_SIZE (8192)

#define SINGLETON(_classname) public:\
								static _classname* GetInstance() \
								{\
									static _classname manager; \
									return &manager;\
								}\
								private:\
									_classname();\
									~_classname();

#define KEYINPUTCHECK(_KEY, _STATE) CKeyManager::GetInstance()->GetKeyState(_KEY) == _STATE

#define KEYINPUTHOLD(_KEY) KEYINPUTCHECK(_KEY, KEY_STATE::KS_HOLD)

#define KEYINPUTTAP(_KEY) KEYINPUTCHECK(_KEY, KEY_STATE::KS_TAP)

#define KEYINPUTAWAY(_KEY) KEYINPUTCHECK(_KEY, KEY_STATE::KS_AWAY)

#define DELTA_F CTimeManager::GetInstance()->GetfDelta()

#define DELTA_D CTimeManager::GetInstance()->GetDelta()

#define MOUSE_POS CKeyManager::GetInstance()->GetMousePos()

