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


enum class GROUP_TYPE { // Scene �ȿ� Object���� ������ �׷��̴�.
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

