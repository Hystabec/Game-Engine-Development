#pragma once
#include "pugixml.hpp"
#include <string>

//#define so that it is quicker the use the Logger instance
#define Debug_Logger Logger::instance()

enum LoggerErrorTypes	//this is public for anything that has logger.h included
{
	Logger_Error = 1,
	Logger_Warning = 2,
	Logger_Info = 4
	//can add more logger levels numbers of the enum need to be bits so the next will be 8,16,32...
};

/// @brief This is a singleton class that will handle all the error logging - either outputting to a file or outputting to the console
class Logger 
{
private:
	enum Verbosity
	{
		None,
		Error,
		Warning,
		Info,
		All
	};

	int global_verbosity = All;
	bool canWriteToConsole = true;

	pugi::xml_document configFile;

	unsigned char fileFlags = (char)0;
	unsigned char consoleFlags = (char)0;

	/// @brief This will take a string out write it to the error file
	/// @param messageToOutput - the string that needs to be written to a file
	void outputToFile(std::string messageToOutput);

	/// @brief This is a function that will take a sting of chars as well as an arg list and type and depending on the type output it to different places
	/// @param format - the format of the message to be written
	/// @param argList - the arg list to add to the format
	/// @param type	- the type that will decide where the message is written too
	/// @return int - returns the number of chars written
	int VTypeDebugPrintF(const char* format, va_list argList, LoggerErrorTypes type);

	/// @brief This will parse and XML file and will set the file flags to the values found in the file
	void getFlags();

	/// @return takes a const int time and returns it as a formatted string
	std::string formatTime(const int TimeToFormat);

	static Logger* _instance;
public:

	/// @return returns the instance of the singleton
	static Logger* instance();

	/// @brief this will be used to clear all the error logs
	void clearLogFile();

	/// @brief This will take a format and an arg list outputs them to console 
	/// @param format - the format of the message
	/// @param argList - the args of the message
	/// @return int - the number of chars written
	int VDebugPrintF(const char* format, va_list argList);

	/// @brief This will take a format and an args list and output them to the console
	/// @param format - the format of the message
	/// @param args - the args of the message 
	/// @return int - the number of chars written
	int DebugPrintF(const char* format, ...);

	/// @brief This will take a format and an args list and output it to the console - if the verbosity is greater than the global verboisity nothing will happen
	/// @param verbosity - the verbosity of the message
	/// @param format - the format of the message to output
	/// @param args - the args of the message to output 
	/// @return int - the number of chars written
	int VerbosDebugPrintF(int verbosity, const char* format, ...);

	/// @brief This will take a LoggerErrorType as well as a format and args depending on the type can either write it to a file, the console, both, neither
	/// @param errorType - the type of the message to write
	/// @param format - the format for the message
	/// @param args - the args of the message 
	/// @return int - the number of chars written
	int TypeDebugPrintF(LoggerErrorTypes errorType, const char* format, ...);

	/// @brief Initializes the logger
	void Init();
};