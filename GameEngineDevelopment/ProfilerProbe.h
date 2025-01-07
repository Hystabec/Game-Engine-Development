#pragma once
#include <chrono>
#include <ctime>
#include "Profile.h"

/// @brief This is a struct that is used by the profiler that will start a timer for the current and assign an ID on creation
/// then when ending the profiler it will get all the data calculate the time taken for the profile and put all the data into a profile struct and the return
struct ProfilerProbe
{
	using clock = std::chrono::steady_clock;

	ProfilerProbe(unsigned long startFrame, unsigned int functionID)
	{
		startTime = clock::now();
		this->startFrame = startFrame;
		this->functionID = functionID;
	}

	Profile getProfileData(unsigned long endFrame)
	{
		auto endTime = clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
		Profile resultProfile(startFrame, endFrame, duration.count());
		return resultProfile;
	}

	~ProfilerProbe()
	{

	}
	
	unsigned int functionID;
private:
	std::chrono::steady_clock::time_point startTime;
	unsigned long startFrame;
	
};

