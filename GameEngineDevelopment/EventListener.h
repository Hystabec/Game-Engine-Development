#pragma once
#include "EventHandler.hpp"
#include "EventMarker.h"

/// @brief The EventListener is a wrapper class so that the event system functions are all in one place but not contain in something like a function
class EventListener : public EventHandler
{
public:
	/// @brief The constructer will setup all the things nessacery to listen to and handle an event
	/// @param EventTypeToListenFor - the type of event to listen to e.g. EVENT_INPUT_1
	/// @param typeOfDataToListenFor - the type of data it gives e.g. bool
	/// @param EventMarkerToHoldData - the marker that will get and hold the data
	EventListener(eventType EventTypeToListenFor, Event::TypeOfData typeOfDataToListenFor, EventMarker* EventMarkerToHoldData = new EventMarker());
	~EventListener();

	void onEvent(eventType& thiseventType, Event& eventData);
private:
	eventType listeningEventType;
	Event::TypeOfData listeningDataType;
	EventMarker placeToReturnData;
};