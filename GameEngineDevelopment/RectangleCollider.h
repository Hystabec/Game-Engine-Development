#pragma once
#include "Component.hpp"

struct rectangleBounds;

/// @brief A basic rectangle collider/Trigger that allows basic trigger and physic collisions
class RectangleCollider : public Component
{
public:
	RectangleCollider(bool isTrigger = false, std::string Tag = "Default");
	~RectangleCollider();

	virtual void init() override;
	virtual void update() override;

	virtual std::string getDataToSave() override;

	//these should be private
	bool Trigger;
	std::string Tag;
	std::string currentTriggerTag = "NULL";
};