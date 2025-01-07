#pragma once
#include "I_ComponentGUI.h"

class SpriteRenderer;

class SpriteRendererGUI : public I_ComponentGUI
{
private:
	SpriteRenderer* SpriteRend;
public:
	SpriteRendererGUI(SpriteRenderer* sprite)
	{
		SpriteRend = sprite;
	}
	~SpriteRendererGUI() 
	{
		SpriteRend = nullptr;
	}

	// Inherited via I_ComponentGUI
	virtual const std::string& getComponetName() override;
	virtual void Draw() override;
	virtual ComponetClass getComponetType() override;
	virtual Component* getPtrToBaseComp() override;
};