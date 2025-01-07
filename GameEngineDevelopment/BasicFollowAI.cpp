#include "BasicFollowAI.h"
#include "GameObject.h"
#include "Transform.h"

BasicFollowAI::BasicFollowAI(vector2 pointOne, vector2 pointTwo, int movementSpeed)
{
	//setting default and component values
	nameOfComponent = "BasicFollowAI";
	componetClass = ComponetClass::CC_BasicFollowAI;
	componetType = ComponetType(true, false);

	this->patrolPointone = pointOne;
	this->patrolPointtwo = pointTwo;
	currentPointToMoveTo = 1;
	this->movementSpeed = movementSpeed;
}

BasicFollowAI::~BasicFollowAI()
{

}

void BasicFollowAI::init()
{

}

void BasicFollowAI::update()
{
	//if the two points are the same dont do anything
	if (patrolPointone == patrolPointtwo)
		return;

	//saves X and Y - saves typing "parentGameObject.lock()->getMorphingTransform().Position()" each time
	auto currentX = parentGameObject.lock()->getMorphingTransform().Position().x;
	auto currentY = parentGameObject.lock()->getMorphingTransform().Position().y;

	//checks which its currently moving too
	if (currentPointToMoveTo == 1)
	{
		//theses are used to check if both x and y are the same and switch to the other point if so
		bool xSame = false;
		bool ySame = false;

		//calculates the difference between the current position and the patrol position
		auto differentX = currentX - patrolPointone.x;
		auto differnetY = currentY - patrolPointone.y;

		bool xIsNegative = differentX < 0;
		bool yIsNegatice = differnetY < 0;

		//checks the difference if they are the same do nothing and set same to true
		if (differentX != 0)
		{
			int absx = abs(differentX);
			//checks if the distance is less than the movement speed - this is so it moves to the exact point instead of stepping over it
			if (absx > movementSpeed)
			{
				if(xIsNegative)
					currentX = currentX + movementSpeed;
				else
					currentX = currentX - movementSpeed;

				parentGameObject.lock()->getMorphingTransform().Position().x = currentX;
			}
			else
			{
				if (xIsNegative)
					currentX = currentX - differentX;
				else
					currentX = currentX - differentX;

				parentGameObject.lock()->getMorphingTransform().Position().x = currentX;
			}
		}
		else
			xSame = true;

		//same as above but using y instead
		
		if (differnetY != 0)
		{
			int absy = abs(differnetY);
			if (absy > movementSpeed)
			{
				if (yIsNegatice)
					currentY = currentY + movementSpeed;
				else
					currentY = currentY - movementSpeed;

				parentGameObject.lock()->getMorphingTransform().Position().y = currentY;
			}
			else
			{
				if (yIsNegatice)
					currentY = currentY - differnetY;
				else
					currentY = currentY - differnetY;

				parentGameObject.lock()->getMorphingTransform().Position().y = currentY;
			}
		}
		else
			ySame = true;

		//if both the x and y are the same, at the point and patrol to the other point
		if (xSame && ySame)
			currentPointToMoveTo = 2;
	}
	else if (currentPointToMoveTo == 2)
	{
		//same as above but using patrol point two instead as one 

		bool xSame = false;
		bool ySame = false;

		auto differentX = currentX - patrolPointtwo.x;
		auto differnetY = currentY - patrolPointtwo.y;

		bool xIsNegative = differentX < 0;
		bool yIsNegatice = differnetY < 0;

		
		if (differentX != 0)
		{
			int absx = abs(differentX);
			if (absx > movementSpeed)
			{
				if (xIsNegative)
					currentX = currentX + movementSpeed;
				else
					currentX = currentX - movementSpeed;

				parentGameObject.lock()->getMorphingTransform().Position().x = currentX;
			}
			else
			{
				if (xIsNegative)
					currentX = currentX - differentX;
				else
					currentX = currentX - differentX;

				parentGameObject.lock()->getMorphingTransform().Position().x = currentX;
			}
		}
		else
			xSame = true;

		
		if (differnetY != 0)
		{
			int absy = abs(differnetY);
			if (absy > movementSpeed)
			{
				if (yIsNegatice)
					currentY = currentY + movementSpeed;
				else
					currentY = currentY - movementSpeed;

				parentGameObject.lock()->getMorphingTransform().Position().y = currentY;
			}
			else
			{
				if (yIsNegatice)
					currentY = currentY - differnetY;
				else
					currentY = currentY - differnetY;

				parentGameObject.lock()->getMorphingTransform().Position().y = currentY;
			}
		}
		else
			ySame = true;

		if (xSame && ySame)
			currentPointToMoveTo = 1;
	}
}

vector2 BasicFollowAI::getPositionOne()
{
	return patrolPointone;
}

vector2 BasicFollowAI::getPositionTwo()
{
	return patrolPointtwo;
}

int BasicFollowAI::getMovementSpeed()
{
	return movementSpeed;
}

void BasicFollowAI::changePointOne(vector2 newPoint)
{
	this->patrolPointone = newPoint;
}

void BasicFollowAI::changePointTwo(vector2 newPoint)
{
	this->patrolPointtwo = newPoint;
}

void BasicFollowAI::changeMovementSpeed(int newSpeed)
{
	this->movementSpeed = newSpeed;
}

std::string BasicFollowAI::getDataToSave()
{
	//this will create and return a string that can be used by the save and load system to recreate to component
	std::string returnString = isActive ? "true" : "false";
	returnString += "=";
	returnString += std::to_string(patrolPointone.x);
	returnString += "=";
	returnString += std::to_string(patrolPointone.y);
	returnString += "=";
	returnString += std::to_string(patrolPointtwo.x);
	returnString += "=";
	returnString += std::to_string(patrolPointtwo.y);
	returnString += "=";
	returnString += std::to_string(movementSpeed);

	return returnString;
}
