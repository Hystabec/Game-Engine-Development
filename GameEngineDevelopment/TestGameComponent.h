#pragma once
#include "Component.hpp"

/// @brief A basic componet That allows so test gameplay
/// when touching a key delete the key and door
/// when touching a portal or enemy it will reset the level
class TestGameComponent : public Component
{
public:
	TestGameComponent();
	~TestGameComponent();

	virtual void init() override;
	virtual void update() override;
};