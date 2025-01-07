#include "EventManager.h"
#include "EventHandler.hpp"
#include "Event.h"

//typedef std::map<eventType, std::vector<EventHandler*>> eventMap;

EventManager* EventManager::_instance = 0;

EventManager* EventManager::instance()
{
	if (!_instance)
	{
		_instance = new EventManager();
	}

	return _instance;
}

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
	for(int index = 0; index < EVENT_TYPE_COUNT; index++)
	{
		for (auto value : listeners[eventType(index)])
		{
			value = nullptr;
		}
	}
}

bool EventManager::AddListener(eventType type, EventHandler* listener)
{
	for (EventHandler* EHP : listeners[type])
	{
		//goes through each of the listeners looking to see if it is already there
		if (EHP == listener)
		{
			//if found it will do nothing and return false;
			return false;
		}
	}

	//if it isnt found then it will push the listener onto the type vector 
	listeners[type].push_back(listener);
	return true;
}

bool EventManager::RemoveListener(eventType type, EventHandler* listerner)
{
	int index = 0;
	for (EventHandler* EHP : listeners[type])
	{
		if (EHP == listerner)
		{
			auto itor = listeners[type].begin() + index;
			listeners[type].erase(itor);
			return true;
		}
		index++;
	}

	return false;
}

void EventManager::FireEvent(eventType type, Event* event)
{
	for (EventHandler* handler : listeners[type])
	{
		handler->onEvent(type, *event);
	}
}
