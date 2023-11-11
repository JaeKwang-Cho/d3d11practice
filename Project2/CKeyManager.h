#pragma once

class CKeyManager
{
	SINGLETON(CKeyManager)

private:
	vector<tKeyInfo> m_VecKey;

public:
	void Init();
	void Update();
	KEY_STATE GetKeyState(KEY _key)
	{
		return m_VecKey[(int)_key].eState;
	}

	friend class CMouseManager;
};
