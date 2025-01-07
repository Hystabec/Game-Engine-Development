#pragma once

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <atomic>
#include "Profile.h"

#define profiler Profiler::instance()

struct ProfilerProbe;

class Profiler
{
private:
	typedef std::vector<std::map<std::string, Profile>> vectorOfMapProfiles;
	typedef std::vector<std::map<std::string, ProfilePlus>> vectorOfMapProfilesPlus;
	struct pointerToMap;

	static Profiler* _instance;
	unsigned long frameNumber = 0;

	const unsigned int maxBufferSize = 100;
	unsigned int currentBufferCount = 0;
	
	std::string folderLocation = "../Profiles/";
	std::string fileExtention = ".engineProfile";
	std::map<std::string, ProfilerProbe*> profileProbeMap;
	unsigned int currentFunctionID = 0;

	std::map<std::string, ProfilePlus> currentFrameMap;
	std::vector<FrameData> MapOfDataToRetain;

	vectorOfMapProfiles MapBufferOne;
	vectorOfMapProfiles MapBufferTwo;
	vectorOfMapProfiles* readMap;
	vectorOfMapProfiles* writeMap;

	bool isActive = true;

	/// @brief This function will be run on a seperate thread that will always be running it the background, so lots of time isnt wasted on the main thread writing to a file
	/// @param ptrToBufferOne - This is a pointer to vectorOfMapProfiles that will be the first buffer
	/// @param ptrToBufferTwo - This is a pointer to vectorOfMapProfiles that will be the second buffer
	/// @param bufferToRead - this is a ref to a int to say which buffer to read from
	/// @param canWrite - this is a bool to say if there is a buffer ready to output its data to a file
	/// @param keepThreadOpen - this is a bool that once set to false the thread will stop
	void ThreadedOutputToFile(vectorOfMapProfiles& ptrToBufferOne, vectorOfMapProfiles& ptrToBufferTwo, int& bufferToRead, bool& canWrite, bool& keepThreadOpen);
	std::thread profileThread;
	bool canWrite = false;
	bool keepThreadOpen = true;
	std::mutex mapMutex;
	int bufferNumToRead = 2;

	/// @brief This function should be used on close just to write any data that is remaining in the buffer - multithreading was added to take its place
	void outputDataToFile(vectorOfMapProfiles& mapToOutput, bool beingUsedOnClose = false);
	/// @brief This should only be used on clise - This will call outputDataToFile on the two buffer so any data remaining in the buffers is added to the file
	void saveDateBeforeDestruction();
	unsigned int currentMaxDepth = 0;
public:
	Profiler();
	~Profiler();

	/// @return returns a pointer to the instance of the singleton
	static Profiler* instance();

	/// @brief This is used when starting a profile - should be followed by endProfile
	/// @param Name - the name of the profile you want to start
	void startProfile(std::string Name);

	/// @brief This should be used following startProfile - to save a finished profile
	/// @param Name - the name of the profile you want to end
	void endProfile(std::string Name);

	/// @brief should only be used at the end of main update loop - increments the current frame it
	void nextFrame();

	/// @brief allows enableing and disableing of the profiler
	/// @param activeBool - the new bool you want to set it too
	void setActive(bool activeBool);

	/// @return bool - current active status of the profiler
	const bool ActiveStatus();

	/// @return std::vector<FrameData> - returns the vector of all the stored frame data
	const std::vector<FrameData>& getStoredProfileMap();

	/// @return const unsigned long - reuturns the current frame number
	const unsigned long& getRefToCurrentFrame();

	/// @brief This should be when getting the current frame data
	/// @param getTrueCurrentFrame  - this is a bool if true will will return the data of the frame currently being made, if false will return the completed previous frame data
	/// @return FrameData - All the data for the profiles of that frame in a struct
	const FrameData& getCurrentFrameData(bool getTrueCurrentFrame = false);
};