#pragma once

#define MOUSE_THRESHOLD (0.001f)

class CMouseManager
{
	SINGLETON(CMouseManager)
	// 마우스 탄도학 적용하기
private:
	FLOAT2 m_vMousePos;

	FLOAT2 m_vRelativeMouseMov;
public:
	void Init();
	void Update();
	void Render(bool _bRelative);

	void MouseMoved(_In_ const BYTE* lpb);
	void RezeroRelativePos();

	FLOAT2 GetMousePos()
	{
		return m_vMousePos;
	}

	FLOAT2 GetRelativePos()
	{
		return m_vRelativeMouseMov;
	}

	void SetCursorMiddle();

	friend class CKeyManager;
};

