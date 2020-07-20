#pragma once

struct Telegram
{
	int sender;
	int receiver;
	int msg;

	double dispatchTime;
	void* extraInfo;

	Telegram() : dispatchTime(-1), sender(-1), receiver(-1), msg(-1) {}

	Telegram(double time, int sender, int receiver, int msg, void* info = NULL) : 
		dispatchTime(time), sender(sender), receiver(receiver), msg(msg), extraInfo(info) {}

};

inline bool operator==(const Telegram& t1, const Telegram& t2)
{
	return (t1.sender == t2.sender) &&
		(t1.receiver == t2.receiver) &&
		(t1.msg == t2.msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
	if (t1 == t2)
		return false;
	else if (t1.dispatchTime == t2.dispatchTime)
		return (&t1.dispatchTime < &t2.dispatchTime);
	else
		return (t1.dispatchTime < t2.dispatchTime);
}

inline std::ostream& operator<<(std::ostream& os, const Telegram& t)
{
	os << "time: " << t.dispatchTime << "  Sender: " << t.sender
		<< "   Receiver: " << t.receiver << "   Msg: " << t.msg;

	return os;
}