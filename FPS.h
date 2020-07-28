#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class FPS : public MonoBehavior<FPS>
{
private /*�� ������ private ������ �����ϼ���.*/:
	wstring* text;
public  /*�� ������ public ������ �����ϼ���.*/:

private:
	friend class GameObject;
	friend class MonoBehavior<FPS>;
	FPS() = default;
	FPS(FPS&) = default;

public:
	~FPS() {}

	void Start(/*�ʱ�ȭ �ڵ带 �ۼ��ϼ���.*/)
	{
		text = &gameObject->GetComponent<Text>()->text;
	}

	void Update(/*������Ʈ �ڵ带 �ۼ��ϼ���.*/)
	{
		text[0] = std::to_wstring(Time::Instance()->GetFrameRate()) + L"FPS";
		//_itow_s(Time::Instance()->GetFrameRate(), text[0], 0);

		//_itow_s(m_nCurrentFrameRate, lpszString, nCharacters, 10);
		//text[0] = CyanFW::Instance()->m_pszFrameRate;

	}
	// �ʿ��� ��� �Լ��� ���� �� ���� �ϼŵ� �˴ϴ�.
};