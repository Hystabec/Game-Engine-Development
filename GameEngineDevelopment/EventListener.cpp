#include "EventListener.h"
#include "EventManager.h"
#include "Logger.h"

//#define EM EventManager::instance()

EventListener::EventListener(eventType EventTypeToListenFor, Event::TypeOfData typeOfDataToListenFor, EventMarker* EventMarkerToHoldData)
{
	Event_Manager->AddListener(EventTypeToListenFor, this);
    listeningEventType = EventTypeToListenFor;
    listeningDataType = typeOfDataToListenFor;
	placeToReturnData = *EventMarkerToHoldData;
}

EventListener::~EventListener()
{
	Event_Manager->RemoveListener(listeningEventType, this);
}

void EventListener::onEvent(eventType& thiseventType, Event& eventData)
{
	if (thiseventType == listeningEventType)
	{
		switch (listeningDataType)
		{
		case Event::TYPE_INT:
			placeToReturnData.updataData(eventData.asINT);
			break;
		case Event::TYPE_FLOAT:
			placeToReturnData.updataData(eventData.asFLOAT);
			break;
		case Event::TYPE_BOOL:
			placeToReturnData.updataData(eventData.asBOOL);
			break;
		case Event::TYPE_STRING:
			Debug_Logger->TypeDebugPrintF(Logger_Warning, "cant return a string outside of component onEvent()");
			break;
		default:
			break;
		}
	}
}
