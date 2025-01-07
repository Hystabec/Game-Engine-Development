#pragma once
#include "Event.h"

/// @brief this will be used if an event needs to be triggered and needs data outside of a component
///	this will probably not be used too much as most onEvents will be contained in components
/// 
class EventMarker
{
public:
	enum EventMarkerDataType
	{
		EVENT_MARKER_TYPE_INT,
		EVENT_MARKER_TYPE_FLOAT,
		EVENT_MARKER_TYPE_BOOL,
		EVENT_MARKER_TYPE_COUNT
	};

	/// @brief create a marker of the type passed in
	/// @param dataType - the type to listen for e.g. EVENT_MARKER_TYPE_FLOAT
	EventMarker(EventMarkerDataType dataType = EVENT_MARKER_TYPE_COUNT);
	~EventMarker();

	/// @brief updates the data in the marker
	/// @param data - the type of data to update
	void updataData(std::variant<int, float, bool> data);

	/// @return EventMarkerDataType - an enum class that holds the type of data it is a marker for
	EventMarkerDataType getEventMarkerDataType();

	/// @return gets the data out of the marker based on the typeOfData saved in the marker
	std::variant<int, float, bool> getData();
private:
	void* pointerToData;
	EventMarkerDataType typeOfData;
};