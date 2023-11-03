#include "pch.h"
#include "CTimeManager.h"

CTimeManager::CTimeManager()
	: m_llCurCounter{}
	, m_llFrequency{}
	, m_llPrevCounter{}
	, m_dDT(0.)
	, m_dAccumul(0.)
	, m_iCallCount(0)
	, m_iFPS(0)
{


}

CTimeManager::~CTimeManager()
{

}

void CTimeManager::Init()
{
	// 이렇게 개수를 유니온 변수를 넘겨줘서
	// 
	// 현재 count 값을 가져오고
	QueryPerformanceCounter(&m_llPrevCounter);

	// 1초에 몇개인지 구해와야 한다.
	QueryPerformanceFrequency(&m_llFrequency);

	// 보통 1초에 1000만이다. 그래서 초당 1000인 GetTickCount보다 훨씬 정확하다.
}

void CTimeManager::Update()
{
	QueryPerformanceCounter(&m_llCurCounter);

	// 이전 프레임의 카운팅과, 현재 프레임 카운팅 값의 차이를 구한다.
	// 그리고 1초가 지날 동안 카운트 값으로 나누면, delta가 나오게 된다.
	m_dDT = static_cast<double>(m_llCurCounter.QuadPart - m_llPrevCounter.QuadPart) / static_cast<double>(m_llFrequency.QuadPart);

	m_llPrevCounter = m_llCurCounter;

#ifdef _DEBUG
	if (m_dDT > (1. / 60))
	{
		m_dDT = (1. / 60);
	}
#endif
}

void CTimeManager::Render()
{
	m_iCallCount++;
	m_dAccumul += m_dDT;

	if (m_dAccumul >= 1.)
	{
		m_iFPS = m_iCallCount;
		m_dAccumul = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DT : %f", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInstance()->GetMainHwnd(), szBuffer);
	}
}
