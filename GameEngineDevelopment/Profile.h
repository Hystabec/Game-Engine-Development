#pragma once

/// @brief A stuct that will hold the startFrame, EndFrame, and timetaken of a profiled function
struct Profile
{
	//timeTaken is stored in microseconds

	Profile()
	{
		startFrame = 0;
		endFrame = 0;
		timeTaken = 0;
	}

	Profile(unsigned long startFrame, unsigned long endFrame, unsigned long timeTaken)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->timeTaken = timeTaken;
	}

	unsigned long startFrame = 0;
	unsigned long endFrame = 0;
	unsigned long timeTaken = 0;
};

/// @brief This is an extended version of the ProfileStruct That also holds its depth in the stack, ID of the profiled function and the ID of any function above it
struct ProfilePlus
{
	//timeTaken is stored in microseconds
	//this is very similar to the Profile struct but it will also hold its depth in the exe stack and will be held for the whole program and will be used in the flame graph
	ProfilePlus()
	{
	}

	ProfilePlus(unsigned long startFrame, unsigned long endFrame, unsigned long timeTaken, unsigned int stackDepth, unsigned int functionID)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->timeTaken = timeTaken;
		this->stackDepth = stackDepth;
		this->functionID = functionID;
		this->functionIDAbove = functionID;
	}

	ProfilePlus(unsigned long startFrame, unsigned long endFrame, unsigned long timeTaken, unsigned int stackDepth, unsigned int functionID, unsigned int functionIDAbove)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->timeTaken = timeTaken;
		this->stackDepth = stackDepth;
		this->functionID = functionID;
		this->functionIDAbove = functionIDAbove;
	}

	ProfilePlus(Profile& profileToExtend, unsigned int stackDepth, unsigned int functionID)
	{
		this->startFrame = profileToExtend.startFrame;
		this->endFrame = profileToExtend.endFrame;
		this->timeTaken = profileToExtend.timeTaken;
		this->stackDepth = stackDepth;
		this->functionID = functionID;
		this->functionIDAbove = functionID;
	}

	ProfilePlus(Profile& profileToExtend, unsigned int stackDepth, unsigned int functionID, unsigned int functionIDAbove)
	{
		this->startFrame = profileToExtend.startFrame;
		this->endFrame = profileToExtend.endFrame;
		this->timeTaken = profileToExtend.timeTaken;
		this->stackDepth = stackDepth;
		this->functionID = functionID;
		this->functionIDAbove = functionIDAbove;
	}

	unsigned long startFrame = 0;
	unsigned long endFrame = 0;
	unsigned long timeTaken = 0;
	unsigned int stackDepth = 0;
	unsigned int functionID = 0;
	unsigned int functionIDAbove = 0;
};

/// @brief A stuct that holds the name of a profile as well as the profile it's self
struct Sample
{
	Sample(std::string sameName, ProfilePlus profile)
	{
		this->sampleName = sameName;
		this->profile = profile;
	}

	std::string sampleName;
	ProfilePlus profile;
};

/// @brief A struct to hold a frames worth of Samples as well as the maximum stack depth for that frame
struct FrameData
{
	FrameData(unsigned int maxDepth, Sample sample)
	{
		this->maxDepth = maxDepth;
		this->sampleVector.push_back(sample);
	}

	FrameData(unsigned int maxDepth, std::vector<Sample> sampleVector)
	{
		this->maxDepth = maxDepth;
		this->sampleVector = sampleVector;
	}

	FrameData(unsigned int maxDepth, std::map<std::string, ProfilePlus>& mapOfProfiles)
	{
		this->maxDepth = maxDepth;

		for (std::map<std::string, ProfilePlus>::iterator itor = mapOfProfiles.begin(); itor != mapOfProfiles.end(); itor++)
		{
			sampleVector.push_back(Sample(itor->first, itor->second));
		}
	}

	FrameData(unsigned int maxDepth, std::string profileName, ProfilePlus profile)
	{
		this->maxDepth = maxDepth;
		sampleVector.push_back(Sample(profileName, profile));
	}

	unsigned int maxDepth = 0;
	std::vector<Sample> sampleVector;
};