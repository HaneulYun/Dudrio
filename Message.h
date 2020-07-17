#pragma once

enum message_type
{
	// Idle 상태일 때, 움직이라는 메시지
	Msg_Move,
	Msg_Sleep,
	Msg_WakeUp,
	Msg_Build
};

struct BuildMessageInfo
{
	Vector2 pos;
	BuildingType type;
};