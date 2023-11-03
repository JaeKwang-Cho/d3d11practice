#pragma once
class CTimeManager
{
	SINGLETON(CTimeManager)
private:
	// ū ���ڸ� ���� �ڷ����� ����.
	LARGE_INTEGER m_llCurCounter; // union�̶�� �� ����ü�� ��Ʈ�� �ɰ��� ���� �׷��� ��ﳯ ���̴�.
	LARGE_INTEGER m_llPrevCounter;
	LARGE_INTEGER m_llFrequency;

	double	m_dDT; // ������ ���� �ð� ��
	double m_dAccumul; // ��Ÿ ������
	UINT	m_iCallCount; // �Լ� ȣ�� Ƚ��
	UINT	m_iFPS;	// �ʴ� ȣ�� Ƚ��

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

