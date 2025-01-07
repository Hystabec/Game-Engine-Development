#pragma once

#include "Event.h"
#include "EventTypes.h"

/// @brief anything that needs to handle and event should inherit from this and have the onEvent function
class EventHandler
{
public:
	virtual void onEvent(eventType& eventType, Event& eventData)
	{
		
	}
};