#pragma once

class RectangleCollider;
class GameObject;

/// @brief This is a return struct that hold data on has something collided and if so what it collided with - used by collider component
struct ColliderReturn
{
	ColliderReturn(bool collided, std::weak_ptr<GameObject> otherCollider)
	{
		this->collided = collided;
		this->otherCollider = otherCollider;
	}

	ColliderReturn()
	{
		collided = false;
	}

	bool collided;
	std::weak_ptr<GameObject> otherCollider;
};

/// @brief This is a basic Trigger return struct that will hold data on if something is in the trigger, what it is, and what its tag is - used by collider component
struct TriggerReturn
{
	TriggerReturn(bool inTrigger, std::weak_ptr<GameObject> trigger, std::string triggerTag)
	{
		this->inTrigger = inTrigger;
		this->triggerOwner = trigger;
		this->triggerTag = triggerTag;
	}

	TriggerReturn()
	{
		this->inTrigger = false;
	}

	bool inTrigger;
	std::weak_ptr<GameObject> triggerOwner;
	std::string triggerTag;
};