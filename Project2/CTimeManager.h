#pragma once
class CTimeManager
{
	SINGLETON(CTimeManager)
private:
	// 큰 숫자를 위한 자료형을 쓴다.
	LARGE_INTEGER m_llCurCounter; // union이라고 그 구조체를 비트로 쪼개서 쓰는 그런거 기억날 것이다.
	LARGE_INTEGER m_llPrevCounter;
	LARGE_INTEGER m_llFrequency;

	double	m_dDT; // 프레임 간의 시간 값
	double m_dAccumul; // 델타 누적값
	UINT	m_iCallCount; // 함수 호출 횟수
	UINT	m_iFPS;	// 초당 호출 횟수

public:
	void Init();

	void Update();

	void Render();

public:
	double GetDelta()
	{
		return m_dDT;
	}
	float GetfDelTa()
	{
		return static_cast<float>(m_dDT);
	}
};

