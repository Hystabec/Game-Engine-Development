#pragma once
#include "I_ComponentGUI.h"

class BasicFollowAI;

class BasicFollowAIGUI : public I_ComponentGUI
{
private:
	BasicFollowAI* BFA;
public:
	BasicFollowAIGUI(BasicFollowAI* basicFollowAi)
	{
		BFA = basicFollowAi;
	}

	~BasicFollowAIGUI()
	{
		BFA = nullptr;
	}

	virtual const std::string& getComponetName() override;
	virtual void Draw() override;
	virtual ComponetClass getComponetType() override;
	virtual Component* getPtrToBaseComp() override;
};