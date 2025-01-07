#include "Logger.h"
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

void Logger::clearLogFile()
{
	fstream logFile;
	string fileLocation = "console.log";

	logFile.open(fileLocation, std::fstream::out | std::fstream::trunc);
	logFile.close();
}

int Logger::VDebugPrintF(const char* format, va_list argList)
{
	const unsigned int MAX_CHARS = 1024;
	static char s_buffer[MAX_CHARS];

	int charsWritten = vsnprintf(s_buffer, MAX_CHARS, format, argList);

	if (canWriteToConsole) 
	{
		OutputDebugStringA(s_buffer);

		return charsWritten;
	}
	else
	{
		return 0;
	}
}

int Logger::DebugPrintF(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);

	int charsWritten = VDebugPrintF(format, argList);

	va_end(argList);

	return charsWritten;
}

int Logger::VerbosDebugPrintF(int verbosity, const char* format, ...)
{
	if(global_verbosity < verbosity)
		return 0;

	va_list argList;
	va_start(argList, format);

	int charsWritten = VDebugPrintF(format, argList);

	va_end(argList);

	return charsWritten;
}

int Logger::TypeDebugPrintF(LoggerErrorTypes errorType, const char* format, ...)
{
	va_list argList;
	va_start(argList, format);

	int charsWritten = VTypeDebugPrintF(format, argList, errorType);

	va_end(argList);

	return charsWritten;
}

int Logger::VTypeDebugPrintF(const char* format, va_list argList, LoggerErrorTypes type)
{
	const unsigned int MAX_CHARS = 1024;
	static char s_buffer[MAX_CHARS];

	int charsWritten = vsnprintf(s_buffer, MAX_CHARS, format, argList);

	if ((fileFlags & type) > 0)
	{
		outputToFile(s_buffer);
	}

	if ((consoleFlags & type) > 0)
	{
		if (canWriteToConsole)
		{
			OutputDebugStringA(s_buffer);
		}
	}

	return charsWritten;
}

//hash mapping/dictionary so something like the textureManager which also uses mapping
void Logger::Init()
{	
	const char source[] = "config.xml";
	
	if (!configFile.load_file(source))
	{
		printf("File: %s", source, " could not be loaded!\n");
	}
	else
	{
		getFlags();
	}

	outputToFile("-----------------New Program Instance-----------------\n");
}
	
void Logger::getFlags()
{
	pugi::xml_node categories = configFile.child("categories");

	const char* writeToFile = categories.child("error").child("write_to_file").child_value();

	if (*writeToFile == '1');
		fileFlags |= 1 << 0;

	writeToFile = NULL;

	const char* writeToConsole = categories.child("error").child("write_to_console").child_value();

	if (*writeToConsole == '1')
		consoleFlags |= 1 << 0;

	writeToConsole = NULL;

	writeToFile = categories.child("warning").child("write_to_file").child_value();

	if (*writeToFile == '1')
		fileFlags |= 1 << 1;

	writeToFile = NULL;

	writeToConsole = categories.child("warning").child("write_to_console").child_value();

	if (*writeToConsole == '1')
		consoleFlags |= 1 << 1;

	writeToConsole = NULL;

	writeToFile = categories.child("info").child("write_to_file").child_value();

	if (*writeToFile == '1')
		fileFlags |= 1 << 2;

	writeToFile = NULL;

	writeToConsole = categories.child("info").child("write_to_console").child_value();

	if (*writeToConsole == '1')
		consoleFlags |= 1 << 2;

	writeToConsole = NULL;

	/* ^^^^^ above gets the values that are saved into the config.xml file |WORKING| ^^^^^ */
}

string Logger::formatTime(const int TimeToFormat)
{
	stringstream ss;
	ss << setw(2) << setfill('0') << TimeToFormat;

	return ss.str();
}

void Logger::outputToFile(string messageToOutput)
{
	fstream logFile;		
	string fileLocation = "console.log";	//sets the file location

	logFile.open(fileLocation, ios::app);	//opens the file using the location || app means append so it will be added on to the bottom

	SYSTEMTIME lt;		//these are needed to get the current time 
	GetLocalTime(&lt);

	int hour = lt.wHour;
	int minute = lt.wMinute;
	int second = lt.wSecond;


	if (logFile.is_open())	//checks if the file is open
	{
		logFile << "[" << formatTime(hour) << ":" << formatTime(minute) << ":" << formatTime(second) << "]" << " " << messageToOutput;		//formats that line in the file to have [time] "message"  -- when i move this to a funciton tempoutput can have the error message passed in 
	}
	else
	{
		DebugPrintF("Unable to load: %s", fileLocation);
	}
	logFile.close();
}

Logger* Logger::_instance = NULL;

Logger* Logger::instance()	//make it a singleton
{
	if (!_instance)
	{
		_instance = new Logger();
		_instance->Init();
	}
	return _instance;
}