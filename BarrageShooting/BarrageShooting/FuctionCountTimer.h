#pragma once
#include <iostream>
#include <chrono>

class FuctionCountTimer
{
public:
	FuctionCountTimer();
	FuctionCountTimer(std::string inTimerName);
	~FuctionCountTimer();
private:
	std::chrono::system_clock::time_point start;
	std::string mStartMessage;
	std::string mStopMessage;
};

