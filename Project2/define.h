#pragma once

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

#define DELTA_D CTimeManager::GetInstance()->GetDelTa()

#define MOUSE_POS CKeyManager::GetInstance()->GetMousePos()

enum class GROUP_TYPE { // Scene 안에 Object들을 나누는 그룹이다.
	Default,

	END = 32,
};

enum class SCENE_TYPE {
	Default,

	END,
};
