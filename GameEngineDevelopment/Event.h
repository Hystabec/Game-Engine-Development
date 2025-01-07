#pragma once
#include <string>
#include <variant>


/// @brief The struct for getting the data from a fired event
struct Event
{
public:
	enum TypeOfData
	{
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_BOOL,
		TYPE_STRING,
		TYPE_COUNT
	};

	TypeOfData _type;

	union
	{
		int asINT;
		float asFLOAT;
		bool asBOOL;
		std::string asSTRING;
	};

	Event(TypeOfData type, std::variant<int,float,bool,std::string> data)
	{
		_type = type;
		
		switch (type)
		{
		case(TYPE_INT):
			asINT = std::get<int>(data);
			break;
		case(TYPE_FLOAT):
			asFLOAT = std::get<float>(data);
			break;
		case(TYPE_BOOL):
			asBOOL = std::get<bool>(data);
			break;
		case(TYPE_STRING):
			asSTRING = std::get<std::string>(data);
			break;
		default:
			break;
		}
	}
};