#include "EventMarker.h"

EventMarker::EventMarker(EventMarkerDataType dataType)
{
	switch (dataType)
	{
	case EventMarker::EVENT_MARKER_TYPE_INT:
		pointerToData = malloc(sizeof(int));
		break;
	case EventMarker::EVENT_MARKER_TYPE_FLOAT:
		pointerToData = malloc(sizeof(float));
		break;
	case EventMarker::EVENT_MARKER_TYPE_BOOL:
		pointerToData = malloc(sizeof(bool));
		break;
	default:
		break;
	}
	typeOfData = dataType;
}

EventMarker::~EventMarker()
{
	int* tempI = nullptr;
	float* tempF = nullptr;
	bool* tempB = nullptr;

	if (pointerToData != nullptr)
	{
		delete pointerToData;

		pointerToData = nullptr;
	}
}

void EventMarker::updataData(std::variant<int, float, bool> data)
{
	switch (typeOfData)
	{
	case EventMarker::EVENT_MARKER_TYPE_INT:
		*(int*)pointerToData = std::get<int>(data);
		break;
	case EventMarker::EVENT_MARKER_TYPE_FLOAT:
		*(float*)pointerToData = std::get<float>(data);
		break;
	case EventMarker::EVENT_MARKER_TYPE_BOOL:
		*(bool*)pointerToData = std::get<bool>(data);
		break;
	default:
		break;
	}
}

EventMarker::EventMarkerDataType EventMarker::getEventMarkerDataType()
{
	return typeOfData;
}

std::variant<int, float, bool> EventMarker::getData()
{
	switch (typeOfData)
	{
	case EventMarker::EVENT_MARKER_TYPE_INT:
		return *(int*)pointerToData;
		break;
	case EventMarker::EVENT_MARKER_TYPE_FLOAT:
		return *(float*)pointerToData;
		break;
	case EventMarker::EVENT_MARKER_TYPE_BOOL:
		return *(bool*)pointerToData;
		break;
	default:
		break;
	}
}
