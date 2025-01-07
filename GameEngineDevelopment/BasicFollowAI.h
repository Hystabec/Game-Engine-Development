#pragma once
#include "Component.hpp"
#include "Vector.h"

/// @brief This is a basic AI component that will move a gameObject between two points in the world
class BasicFollowAI : public Component
{
public:
	BasicFollowAI(vector2 pointOne = vector2(0), vector2 pointTwo = vector2(0), int movementSpeed = 5);
	~BasicFollowAI();

	virtual void init() override;
	virtual void update() override;
	
	vector2 getPositionOne();
	vector2 getPositionTwo();
	int getMovementSpeed();

	void changePointOne(vector2 newPoint);
	void changePointTwo(vector2 newPoint);
	void changeMovementSpeed(int newSpeed);

	virtual std::string getDataToSave() override;
private:
	vector2 patrolPointone;
	vector2 patrolPointtwo;
	int currentPointToMoveTo;
	int movementSpeed;
};