#include "RectangleCollider.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "ColliderReturnStruct.h"

RectangleCollider::RectangleCollider(bool isTrigger, std::string Tag)
{
	nameOfComponent = "Rectangle Collider";
	componetType = ComponetType(true, false);
	componetClass = ComponetClass::CC_rectangleCollider;

	Trigger = isTrigger;
	this->Tag = Tag;
}

RectangleCollider::~RectangleCollider()
{
	Collision_Manager->removeCollider(parentGameObject);
}

void RectangleCollider::init()
{
	Collision_Manager->addCollider(parentGameObject);
}

void RectangleCollider::update()
{
	auto triggerReturn = Collision_Manager->inTrigger(this->parentGameObject.lock()->getRectBounds());

	if (triggerReturn.inTrigger)
	{
		currentTriggerTag = triggerReturn.triggerTag;
	}
	else
		currentTriggerTag = "NULL";
}

std::string RectangleCollider::getDataToSave()
{
	std::string returnString = isActive ? "true" : "false";
	returnString += "=";
	returnString += Trigger ? "true" : "false";
	returnString += "=";
	returnString += Tag;
	return (returnString);
}
