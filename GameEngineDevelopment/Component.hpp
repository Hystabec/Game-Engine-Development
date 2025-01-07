#pragma once

#include <memory>
#include "EventHandler.hpp"

class GameObject;

enum class ComponetClass
{
	CC_spriteRenderer,
	CC_basicMovementModule,
	CC_rectangleCollider,
	CC_BasicFollowAI,
	CC_UnknowComponent,
	CC_numberOfCompontClasses
};


struct ComponetType
{
	ComponetType() { isLogicComponet = true; isGraphicsComponet = true; };
	ComponetType(bool isLogic, bool isGraphics) { isLogicComponet = isLogic; isGraphicsComponet = isGraphics; };
	
	bool isLogicComponet;
	bool isGraphicsComponet;
};

/// @brief This is the base class that all component will inherit from
class Component	: public EventHandler
{
public:
	Component() {};
	virtual ~Component() {};

	/// @brief This is the default getter for component names
	/// @return std::string - returns the name stored in the component
	virtual const std::string& getComponentName() { return nameOfComponent; };

	/// @brief  This is the default getter for the component Type
	/// @return ComponentType - Two bools (isLogical, isGraphical)
	virtual const ComponetType& getComponentType() { return componetType; };

	/// @brief This is the default getter for the component Class
	/// @return componentClass - an enum class that says which component
	virtual const ComponetClass& getComponentClass() { return componetClass; };

	//the following functions are not pure virtauls and will need to be overridden if they need to be used in the component - this is because no every child component will need all the functions

	/// @brief By default this will run the init() function if it hasnt already
	virtual void initialize()
	{ 
		if (!initialized)
			init();
	}

	/// @brief This is a virtual function that should be overritten with anything that needs to be ran at the begining of each components
	virtual void init() {};				//this is called once when the program is starting 

	/// @brief This is a virtual function that should be overritten with anything that needs to be ran each update of the game
	virtual void update() {};			//this is called when the program updates every frame (at the middle of the update loop)

	/// @brief This will be called near the end of each frame an should contain any graphical functions that need to be drawn to the screen
	virtual void draw() {};				//this is called when the program updates every frame (at the end of the update loop)

	/// @brief This is a function that will be called by the event manager when the event that is set is fired
	/// @param eventType - This is the event type (enum class) that needs to be listened for
	/// @param eventData - This is a struct that will be the type of data that is returned when the event is fired (e.g. EVENT_INPUT_1 with return a bool)
	virtual void onEvent(eventType& eventType, Event& eventData) {};

	/// @brief This is the default getter for the active status of the component
	/// @return Bool - if the component is active of not
	virtual const bool getActiveStatus() { return isActive; };

	/// @brief This update the active status of the component to a passed in value
	/// @param setActive - changes the active status of the bool to the passed in value
	virtual void setActiveStatus(bool setActive) { isActive = setActive; };

	/// @brief This is used by the save system to get the components data
	/// @return Std::string - a formatted string that will be given to the save system and then saved into a file
	virtual std::string getDataToSave() { return isActive ? "true" : "false"; };	//this will be used when saving scene data and will be unique to each componet - but it will need to format any save data as a string
	
	std::weak_ptr<GameObject> parentGameObject;
protected:
	bool initialized = false;
	bool isActive = true;
	std::string nameOfComponent = "Unknown";
	
	ComponetType componetType = ComponetType();
	ComponetClass componetClass = ComponetClass::CC_UnknowComponent;
};