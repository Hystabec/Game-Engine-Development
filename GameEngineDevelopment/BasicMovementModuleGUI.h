#pragma once
#include "I_ComponentGUI.h"

class BasicMovementModule;

class BasicMovementModuleGUI : public I_ComponentGUI
{
private:
	BasicMovementModule* BMM;
public:
	BasicMovementModuleGUI(BasicMovementModule* basicMovementModule)
	{
		BMM = basicMovementModule;
	}
	~BasicMovementModuleGUI() 
	{
		BMM = nullptr;
	}

	virtual const std::string& getComponetName() override;
	virtual void Draw() override;
	virtual ComponetClass getComponetType() override;
	virtual Component* getPtrToBaseComp() override;
};