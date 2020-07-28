#pragma once
#include "CyanEngine\CyanEngine\framework.h"

class FPS : public MonoBehavior<FPS>
{
private /*이 영역에 private 변수를 선언하세요.*/:
	wstring* text;
public  /*이 영역에 public 변수를 선언하세요.*/:

private:
	friend class GameObject;
	friend class MonoBehavior<FPS>;
	FPS() = default;
	FPS(FPS&) = default;

public:
	~FPS() {}

	void Start(/*초기화 코드를 작성하세요.*/)
	{
		text = &gameObject->GetComponent<Text>()->text;
	}

	void Update(/*업데이트 코드를 작성하세요.*/)
	{
		text[0] = std::to_wstring(Time::Instance()->GetFrameRate()) + L"FPS";
		//_itow_s(Time::Instance()->GetFrameRate(), text[0], 0);

		//_itow_s(m_nCurrentFrameRate, lpszString, nCharacters, 10);
		//text[0] = CyanFW::Instance()->m_pszFrameRate;

	}
	// 필요한 경우 함수를 선언 및 정의 하셔도 됩니다.
};