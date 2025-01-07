#include "CollisionManager.h"

#include "RectangleCollider.h"
#include "ColliderReturnStruct.h"
#include "GameObject.h"
#include "colliderBounds.h"
#include <memory>
#include "Logger.h"
#include <iostream>
#include "Transform.h"


CollisionManager* CollisionManager::_instance = 0;

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

CollisionManager* CollisionManager::instace()
{
	if (!_instance)
	{
		_instance = new CollisionManager();
	}

	return _instance;
}

void CollisionManager::update()
{
	for(std::weak_ptr<GameObject> currentCheck : collidersVector)
	{ 
		auto currentRecCollider = (RectangleCollider*)currentCheck.lock()->findComponent("Rectangle Collider");

		if (currentRecCollider->Trigger)
			continue;

		currentCheck.lock()->updateRectBounds();

		auto colliderReturn = isColliding(currentCheck.lock()->getRectBounds());
		if (colliderReturn.collided)
		{
			if (((RectangleCollider*)colliderReturn.otherCollider.lock()->findComponent("Rectangle Collider"))->Trigger)
				continue;

			auto rectBoundsCheck = currentCheck.lock()->getRectBounds();
			auto rectBoundsOther = colliderReturn.otherCollider.lock()->getRectBounds();

			auto diffrenceVector = rectBoundsCheck.centre - rectBoundsOther.centre;

			//bottom
			int calcVal = ((rectBoundsCheck.origin + rectBoundsCheck.bottomRight).y - (rectBoundsOther.origin + rectBoundsOther.topRight).y);
			if (calcVal >= 0 && calcVal < 0 + rectBoundsOther._height)
			{
				//bool bottom left is between the top left and right of otherCollider
				bool arg1 = ((rectBoundsCheck.origin + rectBoundsCheck.bottomLeft).x >= (rectBoundsOther.origin + rectBoundsOther.topLeft).x) || ((rectBoundsCheck.origin + rectBoundsCheck.bottomLeft).x <= (rectBoundsOther.origin + rectBoundsOther.topRight).x);

				//bool bottom right is between the top left and right of otherCollider
				bool arg2 = ((rectBoundsCheck.origin + rectBoundsCheck.bottomRight).x >= (rectBoundsOther.origin + rectBoundsOther.topLeft).x) || ((rectBoundsCheck.origin + rectBoundsCheck.bottomRight).x <= (rectBoundsOther.origin + rectBoundsOther.topRight).x);

				if (arg1 || arg2)
				{
					currentCheck.lock()->collidedOnBottom = true;
					//currentCheck.lock()->setMophingPossitionTransform(currentCheck.lock()->getMorphingTransform().Position().y + calcVal);
				}
				else
					currentCheck.lock()->collidedOnBottom = false;
			}
			else
				currentCheck.lock()->collidedOnBottom = false;

			//top
			calcVal = ((rectBoundsCheck.origin + rectBoundsCheck.topRight).y - (rectBoundsOther.origin + rectBoundsOther.bottomRight).y);
			if (calcVal <= 0 && calcVal > 0 - rectBoundsOther._height)
			{
				//bool top left is between the bottom left and right of otherCollider
				bool arg1 = ((rectBoundsCheck.origin + rectBoundsCheck.topLeft).x >= (rectBoundsOther.origin + rectBoundsOther.bottomLeft).x) || ((rectBoundsCheck.origin + rectBoundsCheck.bottomLeft).x <= (rectBoundsOther.origin + rectBoundsOther.bottomRight).x);

				//bool bottom right is between the top left and right of otherCollider
				bool arg2 = ((rectBoundsCheck.origin + rectBoundsCheck.topRight).x >= (rectBoundsOther.origin + rectBoundsOther.bottomLeft).x) || ((rectBoundsCheck.origin + rectBoundsCheck.bottomRight).x <= (rectBoundsOther.origin + rectBoundsOther.bottomRight).x);

				if (arg1 || arg2)
				{
					currentCheck.lock()->collidedOnTop = true;
					//currentCheck.lock()->setMophingPossitionTransform(currentCheck.lock()->getMorphingTransform().Position().y + calcVal);
				}
				else
					currentCheck.lock()->collidedOnTop = false;
			}
			else
				currentCheck.lock()->collidedOnTop = false;

			//right side
			calcVal = ((rectBoundsCheck.origin + rectBoundsCheck.topRight).x - (rectBoundsOther.origin + rectBoundsOther.topLeft).x);
			if (calcVal >= 0 && calcVal < 0 + rectBoundsOther._width)
			{
				//bool top right is between the bottom left and top left of other
				bool arg1 = ((rectBoundsCheck.origin + rectBoundsCheck.topRight).y <= (rectBoundsOther.origin + rectBoundsOther.bottomLeft).y || ((rectBoundsCheck.origin + rectBoundsCheck.topRight).y >= (rectBoundsOther.origin + rectBoundsOther.topLeft).y));

				//bool bottom right is between the bottom left and top left of other
				bool arg2 = ((rectBoundsCheck.origin + rectBoundsCheck.bottomRight).y <= (rectBoundsOther.origin + rectBoundsOther.bottomLeft).y || ((rectBoundsCheck.origin + rectBoundsCheck.bottomRight).y >= (rectBoundsOther.origin + rectBoundsOther.topLeft).y));

				if (arg1 || arg2)
				{
					currentCheck.lock()->collidedOnRight = true;
					//currentCheck.lock()->setMophingPossitionTransform(currentCheck.lock()->getMorphingTransform().Position().x + calcVal);
				}
				else
					currentCheck.lock()->collidedOnRight = false;
			}
			else
				currentCheck.lock()->collidedOnRight = false;

			//left side
			calcVal = ((rectBoundsCheck.origin + rectBoundsCheck.topLeft).x - (rectBoundsOther.origin + rectBoundsOther.topRight).x);
			if (calcVal <= 0 && calcVal > 0 - rectBoundsOther._width)
			{
				//bool top left is between the bottom left and top left of other
				bool arg1 = ((rectBoundsCheck.origin + rectBoundsCheck.topLeft).y <= (rectBoundsOther.origin + rectBoundsOther.bottomLeft).y || ((rectBoundsCheck.origin + rectBoundsCheck.topRight).y >= (rectBoundsOther.origin + rectBoundsOther.topLeft).y));

				//bool bottom left is between the bottom left and top left of other
				bool arg2 = ((rectBoundsCheck.origin + rectBoundsCheck.bottomLeft).y <= (rectBoundsOther.origin + rectBoundsOther.bottomLeft).y || ((rectBoundsCheck.origin + rectBoundsCheck.bottomLeft).y >= (rectBoundsOther.origin + rectBoundsOther.topLeft).y));

				if (arg1 || arg2)
				{
					currentCheck.lock()->collidedOnLeft = true;
					//currentCheck.lock()->setMophingPossitionTransform(currentCheck.lock()->getMorphingTransform().Position().x + calcVal);
				}
				else
					currentCheck.lock()->collidedOnLeft = false;
			}
			else
				currentCheck.lock()->collidedOnLeft = false;
			
		}
		else
		{
			currentCheck.lock()->collidedOnBottom = false;
			currentCheck.lock()->collidedOnLeft = false;
			currentCheck.lock()->collidedOnRight = false;
			currentCheck.lock()->collidedOnTop = false;
		}
	}
}

void CollisionManager::addCollider(std::weak_ptr<GameObject> colliderToAdd)
{
	if (colliderToAdd.lock() == nullptr)
	{
		Debug_Logger->TypeDebugPrintF(Logger_Error, "AddCollider() was passed a nullptr");
		return;
	}

	for (int index = 0; index < collidersVector.size(); index++)
	{
		if(collidersVector[index].lock() == colliderToAdd.lock())
			return;
	}

	collidersVector.push_back(colliderToAdd);
}

void CollisionManager::removeCollider(std::weak_ptr<GameObject> colliderToRemove)
{
	for (int index = 0; index < collidersVector.size(); index++)
	{
		if (collidersVector[index].lock() == colliderToRemove.lock())
		{
			collidersVector.erase(collidersVector.begin() + index);
			return;
		}
	}
}

ColliderReturn CollisionManager::isColliding(rectangleBounds colliderToCheck)
{
	for (std::weak_ptr<GameObject> other : collidersVector)
	{
		if (((RectangleCollider*)other.lock()->findComponent("Rectangle Collider"))->Trigger)
			continue;

		other.lock()->updateRectBounds();

		rectangleBounds tempRec = other.lock()->getRectBounds();
		if (tempRec == colliderToCheck)	//dont check itself
			continue;

		bool arg1 = false;
		bool arg2 = false;
		bool arg3 = false;
		bool arg4 = false;

		//bool arg1 = colliderToCheck.topRight.x + colliderToCheck._width >= tempRec.topRight.x;
		//bool arg2 = tempRec.topRight.x + tempRec._width >= colliderToCheck.topRight.x;
		//bool arg3 = colliderToCheck.topRight.y + colliderToCheck._height >= tempRec.topRight.y;
		//bool arg4 = tempRec.topRight.y + tempRec._height >= colliderToCheck.topRight.y;

		//assumming topleft is origin
		arg1 = colliderToCheck.origin.x < tempRec.origin.x + tempRec._width;
		arg2 = colliderToCheck.origin.x + colliderToCheck._width > tempRec.origin.x;
		arg3 = colliderToCheck.origin.y + colliderToCheck._height > tempRec.origin.y;
		arg4 = colliderToCheck.origin.y < tempRec.origin.y + tempRec._height;
		

		//AABB
		if ((arg1 &&
			arg2) &&
			(arg3 &&
			arg4))
		{
			return ColliderReturn(true, other);
		}
	}

	return ColliderReturn();
}

TriggerReturn CollisionManager::inTrigger(rectangleBounds colliderToCheck)
{
	for (std::weak_ptr<GameObject> other : collidersVector)
	{
		auto otherCollider = ((RectangleCollider*)other.lock()->findComponent("Rectangle Collider"));
		if (!otherCollider->Trigger)
			continue;

		other.lock()->updateRectBounds();

		rectangleBounds tempRec = other.lock()->getRectBounds();
		if (tempRec == colliderToCheck)	//dont check itself
			continue;

		bool arg1 = false;
		bool arg2 = false;
		bool arg3 = false;
		bool arg4 = false;

		//bool arg1 = colliderToCheck.topRight.x + colliderToCheck._width >= tempRec.topRight.x;
		//bool arg2 = tempRec.topRight.x + tempRec._width >= colliderToCheck.topRight.x;
		//bool arg3 = colliderToCheck.topRight.y + colliderToCheck._height >= tempRec.topRight.y;
		//bool arg4 = tempRec.topRight.y + tempRec._height >= colliderToCheck.topRight.y;

		//assumming topleft is origin
		arg1 = colliderToCheck.origin.x < tempRec.origin.x + tempRec._width;
		arg2 = colliderToCheck.origin.x + colliderToCheck._width > tempRec.origin.x;
		arg3 = colliderToCheck.origin.y + colliderToCheck._height > tempRec.origin.y;
		arg4 = colliderToCheck.origin.y < tempRec.origin.y + tempRec._height;


		//AABB
		if ((arg1 &&
			arg2) &&
			(arg3 &&
				arg4))
		{
			return TriggerReturn(true, other, otherCollider->Tag);
		}
	}

	return TriggerReturn();
}
