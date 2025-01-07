#pragma once
#include "I_ComponentGUI.h"

class AnonComponentGUI : public I_ComponentGUI
{
private:
	Component* compPtr;
public:
	AnonComponentGUI(Component* comp)
	{
		compPtr = comp;
	}
	~AnonComponentGUI()
	{
		compPtr = nullptr;
	}

	virtual const std::string& getComponetName() override;
	virtual void Draw() override;
	virtual ComponetClass getComponetType() override;
	virtual Component* getPtrToBaseComp() override;
};