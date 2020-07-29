#pragma once

struct BuildMessageInfo
{
	Vector2D pos;
	int buildingType;
	int buildingIndex;
};

struct SIM_Message {
	int msg;
	void* extra_info;

	SIM_Message() : msg(-1) {}
	SIM_Message(int msg, void* info = NULL) : msg(msg), extra_info(info) {}
};
