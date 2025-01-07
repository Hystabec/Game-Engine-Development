#include "Profiler.h"
#include "ProfilerProbe.h"
#include "Logger.h"
#include "Profile.h"

#include <fstream>
#include <filesystem>
#include <thread>

Profiler* Profiler::_instance = nullptr;

void Profiler::ThreadedOutputToFile(vectorOfMapProfiles& ptrToBufferOne, vectorOfMapProfiles& ptrToBufferTwo, int& bufferToRead, bool& canWrite, bool& keepThreadOpen)
{
	//this function should only be ran by the seperate thread 

	//holds some values for the duration of the thread
	vectorOfMapProfiles* bufferOne = &ptrToBufferOne;
	vectorOfMapProfiles* bufferTwo = &ptrToBufferTwo;
	vectorOfMapProfiles* mapToUse;
	bool& canWriteBool = canWrite;
	bool& keepThreadOpenBool = keepThreadOpen;

	//will loop infiniatly until the keepTheadOpen bool is changed to false on deconstruction of the class
	while (keepThreadOpenBool)
	{
		//this can write bool is here to make sure junk date isnt written to the file and date is only written when the map is full
		if (canWriteBool)
		{
			if (bufferToRead == 1)
				mapToUse = bufferOne;
			//else if (bufferToRead == 2)
			else
				mapToUse = bufferTwo;

			//tries to lock the mutex to make sure that nothing is accessing the data it is trying to read
			if (mapMutex.try_lock())
			{
				//check that the map isnt empty
				if (mapToUse->size() == 0)
				{
					Debug_Logger->TypeDebugPrintF(Logger_Error, "Profiler output - map empty\n");
					mapMutex.unlock();
				}
				else
				{
					//loops over all the element of the same key then will output all its data into a file sharing that profile name then will move onto the rest of the profiles in the map
					for (std::map<std::string, Profile>::iterator profileMap = mapToUse->begin()->begin(); profileMap != mapToUse->begin()->end(); profileMap++)
					{
						std::string fileToOpen = profileMap->first;
						std::string fileName = folderLocation + fileToOpen + fileExtention;
						std::ofstream WriteFile;
						WriteFile.open(fileName, std::ios::app);

						//loops through all the individual frames worth of data and outputs it to a file
						for (vectorOfMapProfiles::iterator iter = mapToUse->begin(); iter != mapToUse->end(); iter++)
						{

							if ((*iter).count(fileToOpen))
							{
								auto currentProfile = &((*iter)[fileToOpen]);
								WriteFile << "Start frame:" << currentProfile->startFrame << ", End frame:" << currentProfile->endFrame << ", Time taken:" << currentProfile->timeTaken << " microseconds\n";
							}
						}
						//closes the file
						WriteFile.close();
					}
					//clears the map, stops it from being able to write data, unlocks the mutex so now it can be editited at the start
					mapToUse->clear();
					canWriteBool = false;
					mapMutex.unlock();
				}
			}
			else
			{
				Debug_Logger->TypeDebugPrintF(Logger_Error, "Profiler thread output mutex in use");
			}
		}
	}
}

void Profiler::outputDataToFile(vectorOfMapProfiles& mapToOutput, bool beingUsedOnClose)
{
	//this should always be ran on close so some code is redundent as threading was added to take its place

	//if the file is empty doesnt need to do anything
	if (mapToOutput.size() == 0)
	{
		if(!beingUsedOnClose)
			Debug_Logger->TypeDebugPrintF(Logger_Error, "Profiler output - map empty\n");

		return;
	}

	if(!beingUsedOnClose)
		startProfile("ProfilerWriteToFile");

	for (std::map<std::string, Profile>::iterator fileNameIter = mapToOutput.begin()->begin(); fileNameIter != mapToOutput.begin()->end(); fileNameIter++)
	{
		std::string fileToOpen = fileNameIter->first;
		std::string fileName = folderLocation + fileToOpen + fileExtention;
		std::ofstream WriteFile;
		WriteFile.open(fileName, std::ios::app);
		for (vectorOfMapProfiles::iterator iter = mapToOutput.begin(); iter != mapToOutput.end(); iter++)
		{
			if ((*iter).count(fileToOpen))
			{
				auto currentProfile = &((*iter)[fileToOpen]);
				WriteFile << "Start frame:" << currentProfile->startFrame << ", End frame:" << currentProfile->endFrame << ", Time taken:" << currentProfile->timeTaken << " microseconds\n";
			}
		}
		WriteFile.close();
	}

	if (!beingUsedOnClose)
		endProfile("ProfilerWriteToFile");

	//then clears the map buffer
	mapToOutput.clear();
}

void Profiler::saveDateBeforeDestruction()
{
	//output any remaining data  that is in the buffers before deleting the profiler
	outputDataToFile(*writeMap, true);
	outputDataToFile(*readMap, true);
}

Profiler::Profiler()
{
	readMap = &MapBufferOne;
	writeMap = &MapBufferTwo;

	//on startup will delete all existing files in the profiles folder so that new ones can be written
	for (const auto& entry : std::filesystem::directory_iterator(folderLocation))
	{
		if (entry.path().extension() == fileExtention)
		{
			std::string filePath = entry.path().generic_string();
			std::remove(filePath.c_str());
		}
	}

	//creates the new thread using the function "ThreadedOutputToFile", with the passed in arguments, "this" is so that it knows which version of the class to get the arguments from
	profileThread = std::thread(&Profiler::ThreadedOutputToFile, this, std::ref(MapBufferOne), std::ref(MapBufferTwo), std::ref(bufferNumToRead), std::ref(canWrite), std::ref(keepThreadOpen));
}

Profiler::~Profiler()
{
	for (auto it = profileProbeMap.begin(); it != profileProbeMap.end(); it++)
	{
		delete it->second;
	}

	keepThreadOpen = false;
	profileThread.join();

	//saveDateBeforeDestruction();
	
	profileProbeMap.clear();
	MapBufferOne.clear();
	MapBufferTwo.clear();
	//profileMap.clear();
	
}

Profiler* Profiler::instance()
{
	if (!_instance)
	{
		_instance = new Profiler();
	}
	return _instance;
}

void Profiler::startProfile(std::string Name)
{
	if (!isActive)
		return;

	if (profileProbeMap.count(Name))
	{
		//is already in map
		Debug_Logger->TypeDebugPrintF(Logger_Error, "Profile probe %s already exists in frame %d\n", Name.c_str(), frameNumber);
		return;
	}
	
	ProfilerProbe* newProbe = new ProfilerProbe(frameNumber, currentFunctionID);
	profileProbeMap[Name] = newProbe;
	currentFunctionID++;
}

void Profiler::endProfile(std::string Name)
{
	if (!isActive)
		return;

	if (!(profileProbeMap.count(Name)))
	{
		//isnt in map
		Debug_Logger->TypeDebugPrintF(Logger_Error, "Mismatch profile probe: no start probe with name %s in frame %d\n", Name.c_str(), frameNumber);
		return;
	}
	//gets data from the probe
	Profile tempProfile = profileProbeMap[Name]->getProfileData(frameNumber);
	if ((*readMap).size() == currentBufferCount)
		(*readMap).push_back({ {Name, tempProfile} });
	else
	{
		auto& temp = (*readMap)[currentBufferCount];
		temp[Name] = tempProfile;
	}

	if (currentMaxDepth < profileProbeMap.size() - 1)
		currentMaxDepth = profileProbeMap.size() - 1;

	unsigned int stackDepth = profileProbeMap.size() - 1;
	unsigned int IDOfFunction = profileProbeMap[Name]->functionID;

	//this is set so that it has a value in it. this will get overwritten if the function happens in another
	unsigned int IDOfFunctionAbove = profileProbeMap[Name]->functionID;

	bool needToGetAbove = false;

	//if the size of the map is more than 1 then this function happens inside another
	if (profileProbeMap.size() > 1)
		needToGetAbove = true;

	//deletes the probe and removes it from the map
	delete profileProbeMap[Name];
	profileProbeMap.erase(Name);

	
	//gets the id of the new final element in the list
	if (needToGetAbove)
		IDOfFunctionAbove = profileProbeMap.begin()->second->functionID;//(--profileProbeMap.end())->second->functionID;

	currentFrameMap[Name] = ProfilePlus(tempProfile, stackDepth, IDOfFunction, IDOfFunctionAbove);
}


void Profiler::nextFrame()
{
	frameNumber++;

	if (!isActive)
		return;

	profiler->startProfile("nextFrame");
	MapOfDataToRetain.push_back(FrameData(currentMaxDepth+1, currentFrameMap));
	currentFrameMap.clear();
	currentMaxDepth = 0;
	currentFunctionID = 0;

	currentBufferCount++;

	if (currentBufferCount > (maxBufferSize-1))
	{
		profiler->startProfile("BufferSwap-Profiler");
		currentBufferCount = 0;

		//will try to use the mutex and will wait until it is avaliable
		while (!mapMutex.try_lock())
		{
			Debug_Logger->TypeDebugPrintF(Logger_Error, "Tried to access locked mutex");
		}

		//this is abit cluncky as it copies all the date from the read map into the write map - this can be solved with thread fences aparently
		//*writeMap = *readMap;

		std::swap(readMap, writeMap);
		
		if (bufferNumToRead == 1)
			bufferNumToRead = 2;
		else
			bufferNumToRead = 1;

		//clears the read map so more data can go into it
		//readMap->clear();

		mapMutex.unlock();

		//sets can write to true so that the other thread can now start writing
		canWrite = true;
		profiler->endProfile("BufferSwap-Profiler");
	}
	profiler->endProfile("nextFrame");
}

void Profiler::setActive(bool activeBool)
{
	isActive = activeBool;
}

const bool Profiler::ActiveStatus()
{
	return isActive;
}

const std::vector<FrameData>& Profiler::getStoredProfileMap()
{
	return MapOfDataToRetain;
}

const unsigned long& Profiler::getRefToCurrentFrame()
{
	return frameNumber;
}

const FrameData& Profiler::getCurrentFrameData(bool getTrueCurrentFrame)
{
	//most of the time you actually want the previous frame which will be a complete profile for everything
	//if you get the "true" current frame it will be incomple as it wont include the main update profile among over things

	if(getTrueCurrentFrame)
		return MapOfDataToRetain[frameNumber];
	else
		return MapOfDataToRetain[frameNumber-1];
}
