#pragma once
#include <map>
#include <vector>
#include "EventTypes.h"
#include "Event.h"
//#include "EventHandler.hpp"

class EventHandler;
//struct Event;

//#define so that it is quicker the use the EventManager instance
#define Event_Manager EventManager::instance()

typedef std::map<eventType, std::vector<EventHandler*>> _eventMap;

/// @brief A singleton that will deal with all the event listening to and fireing through the program
class EventManager
{
private:
	static EventManager* _instance;
public:
	_eventMap listeners;

	/// @return This will return the active instace of the EventManager
	static EventManager* instance();
	EventManager();
	~EventManager();

	/// @brief This will add a listener to the type passed in - so when an event of that type is fire it will be told
	/// @param type - The type of event e.g. EVENT_INPUT_1
	/// @param listener - a ptr to the listener to add
	/// @return bool - true if added, false if already in there
	bool AddListener(eventType type, EventHandler* listener);

	/// @brief This will be when removing a listener - e.g. if a component is being deleted remove it so it doenst call onFire to a deleted object
	/// @param type - The type of event e.g. EVENT_INPUT_1
	/// @param listerner - a ptr the the listerner to remove
	/// @return bool - true if removed, false if not
	bool RemoveListener(eventType type, EventHandler* listerner);

	/// @brief This will be used whenever an event needs to be fired - and will tell all the listeners the event has been fired
	/// @param type - The type of event e.g. EVENT_INPUT_1
	/// @param event - The type of data to send when the event goes of e.g. bool - true
	void FireEvent(eventType type, Event *event);
};