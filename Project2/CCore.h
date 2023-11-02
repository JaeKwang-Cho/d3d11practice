#pragma once

class CCore
{
public:
	SINGLETON(CCore)

private:
	HWND m_hWnd;
	POINT m_ptResolution;

public:
	int Init(HWND _hWnd, POINT _ptResolution);

	void Progress();

private:
	void update();
	void render();

public:
	HWND GetMainHwnd()
	{
		return m_hWnd;
	}

};
