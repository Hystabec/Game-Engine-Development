#pragma once
#include "Component.hpp"

/// @brief A basic component that will move the attached gameObject when W,A,S,D are pressed
class BasicMovementModule : public Component
{
private:
	int ammoutToMove;
	bool wPressed = 0;
	bool aPressed = 0;
	bool sPressed = 0;
	bool dPressed = 0;
public:
	
	BasicMovementModule(int speedToMove = NULL);
	~BasicMovementModule();

	virtual void init() override;
	virtual void update() override;

	virtual void onEvent(eventType& eventType, Event& eventData) override;
	virtual std::string getDataToSave() override;

	int getSpeedToMove();
	void setSpeedToMove(int newSpeedToMove);
};