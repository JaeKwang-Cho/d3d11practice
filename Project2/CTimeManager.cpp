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
	// �̷��� ������ ���Ͽ� ������ �Ѱ��༭
	// 
	// ���� count ���� ��������
	QueryPerformanceCounter(&m_llPrevCounter);

	// 1�ʿ� ����� ���ؿ;� �Ѵ�.
	QueryPerformanceFrequency(&m_llFrequency);

	// ���� 1�ʿ� 1000���̴�. �׷��� �ʴ� 1000�� GetTickCount���� �ξ� ��Ȯ�ϴ�.
}

void CTimeManager::Update()
{
	QueryPerformanceCounter(&m_llCurCounter);

	// ���� �������� ī���ð�, ���� ������ ī���� ���� ���̸� ���Ѵ�.
	// �׸��� 1�ʰ� ���� ���� ī��Ʈ ������ ������, delta�� ������ �ȴ�.
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
