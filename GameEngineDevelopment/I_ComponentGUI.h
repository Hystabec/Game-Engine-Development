#pragma once
#include <string>

enum class ComponetClass;
class Component;

class I_ComponentGUI
{
public:

	virtual ~I_ComponentGUI() {};
	virtual const std::string& getComponetName() = 0;
	virtual void Draw() = 0;
	virtual ComponetClass getComponetType() = 0;
	virtual Component* getPtrToBaseComp() = 0;
};

