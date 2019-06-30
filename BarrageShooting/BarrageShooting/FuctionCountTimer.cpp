#include "stdafx.h"
#include "FuctionCountTimer.h"
#include <string>
#include <sstream>

FuctionCountTimer::FuctionCountTimer()
{
	start = std::chrono::system_clock::now();
}

FuctionCountTimer::FuctionCountTimer(std::string inTimerName)
{
	start = std::chrono::system_clock::now();
	mStartMessage += "###" + inTimerName + "_START###\n";
	mStopMessage += "###" + inTimerName + "_STOP###\n";
	OutputDebugStringA(mStartMessage.c_str());
}

FuctionCountTimer::~FuctionCountTimer()
{
	auto end = std::chrono::system_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::wstringstream ws;
	ws << elapsed << "microseconds" << std::endl;
	OutputDebugString(ws.str().c_str());
	OutputDebugStringA(mStopMessage.c_str());
}

