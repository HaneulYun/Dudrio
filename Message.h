#pragma once

enum message_type
{
	// Idle ������ ��, �����̶�� �޽���
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