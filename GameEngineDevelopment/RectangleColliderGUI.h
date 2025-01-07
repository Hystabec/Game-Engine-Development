#pragma once
#include "I_ComponentGUI.h"

class RectangleCollider;

class RectangleColliderGUI : public I_ComponentGUI
{
private:
	RectangleCollider* rectCol;
public:
	RectangleColliderGUI(RectangleCollider* rectangleCollider)
	{
		rectCol = rectangleCollider;
	}

	~RectangleColliderGUI()
	{
		rectCol = nullptr;
	}

	virtual const std::string& getComponetName() override;
	virtual void Draw() override;
	virtual ComponetClass getComponetType() override;
	virtual Component* getPtrToBaseComp() override;
};

