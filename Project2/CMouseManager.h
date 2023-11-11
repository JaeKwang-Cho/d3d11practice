#pragma once

class CMouseManager
{
	SINGLETON(CMouseManager)
	// ���콺 ź���� �����ϱ�
private:
	FLOAT2 m_vMousePos;

	FLOAT2 m_vRelativeMouseMov;
public:
	void Init();
	void Update();
	void Render();

	void MouseMoved(_In_ const BYTE* lpb);

	FLOAT2 GetMousePos()
	{
		return m_vMousePos;
	}

	FLOAT2 GetRelativePos()
	{
		return m_vRelativeMouseMov;
	}

	friend class CKeyManager;
};

