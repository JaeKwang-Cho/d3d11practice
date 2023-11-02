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