#include "BasicMovementModule.h"
#include "EventManager.h"
#include "Logger.h"

#include "GameObject.h"
#include "Transform.h"

#include "CollisionManager.h"
#include "ColliderReturnStruct.h"

BasicMovementModule::BasicMovementModule(int speedToMove)
{
	nameOfComponent = "Basic Movement Module";
	componetType = ComponetType(true, false);
	componetClass = ComponetClass::CC_basicMovementModule;

	if (speedToMove == NULL)
		ammoutToMove = 5;
	else
		ammoutToMove = speedToMove;

	Event_Manager->AddListener(EVENT_INPUT_W, this);
	Event_Manager->AddListener(EVENT_INPUT_A, this);
	Event_Manager->AddListener(EVENT_INPUT_S, this);
	Event_Manager->AddListener(EVENT_INPUT_D, this);
}

BasicMovementModule::~BasicMovementModule()
{
	Event_Manager->RemoveListener(EVENT_INPUT_W, this);
	Event_Manager->RemoveListener(EVENT_INPUT_A, this);
	Event_Manager->RemoveListener(EVENT_INPUT_S, this);
	Event_Manager->RemoveListener(EVENT_INPUT_D, this);

	parentGameObject.reset();
}

void BasicMovementModule::init()
{

}

void BasicMovementModule::update()
{
	if (!isActive)
		return;

	//auto startTransform = parentGameObject.lock()->getMorphingTransform().Position();

	if (wPressed && !parentGameObject.lock()->collidedOnTop)
	{
		parentGameObject.lock()->getMorphingTransform().Position().y = parentGameObject.lock()->getMorphingTransform().Position().y + -ammoutToMove;
	}

	if (aPressed && !parentGameObject.lock()->collidedOnLeft)
	{
		parentGameObject.lock()->getMorphingTransform().Position().x = parentGameObject.lock()->getMorphingTransform().Position().x + -ammoutToMove;
	}

	if (sPressed && !parentGameObject.lock()->collidedOnBottom)
	{
		parentGameObject.lock()->getMorphingTransform().Position().y = parentGameObject.lock()->getMorphingTransform().Position().y + ammoutToMove;
	}

	if (dPressed && !parentGameObject.lock()->collidedOnRight)
	{
		parentGameObject.lock()->getMorphingTransform().Position().x = parentGameObject.lock()->getMorphingTransform().Position().x + ammoutToMove;
	}

	//auto endTransform = parentGameObject.lock()->getMorphingTransform().Position();

	//parentGameObject.lock()->setAmountMovedThisFrame(endTransform - startTransform);
}

void BasicMovementModule::onEvent(eventType& eventType, Event& eventData)
{
	switch (eventType)
	{
	case(EVENT_INPUT_W):
		wPressed = eventData.asBOOL;
		break;
	case(EVENT_INPUT_A):
		aPressed = eventData.asBOOL;
		break;
	case(EVENT_INPUT_S):
		sPressed = eventData.asBOOL;
		break;
	case(EVENT_INPUT_D):
		dPressed = eventData.asBOOL;
		break;
	default:
		break;
	}
}

std::string BasicMovementModule::getDataToSave()
{
	std::string activeAsString = isActive ? "true" : "false";

	return (activeAsString + "=" + std::to_string(ammoutToMove));
}

int BasicMovementModule::getSpeedToMove()
{
	return ammoutToMove;
}

void BasicMovementModule::setSpeedToMove(int newSpeedToMove)
{
	ammoutToMove = newSpeedToMove;
}
