#include "TestGameComponent.h"

#include "GameObject.h"
#include "sceneHierarchy.h"
#include "RectangleCollider.h"
#include "Logger.h"

TestGameComponent::TestGameComponent()
{
	nameOfComponent = "TestGameComp";
	componetType = ComponetType(true, false);
}

TestGameComponent::~TestGameComponent()
{

}

void TestGameComponent::init()
{

}

void TestGameComponent::update()
{
	// lots of string comparisions (use hashing/enums?) running out of time
	auto rectCollider = ((RectangleCollider*)parentGameObject.lock()->findComponent("Rectangle Collider"));

	//checks a collider is found
	if (rectCollider == nullptr)
	{
		Debug_Logger->TypeDebugPrintF(Logger_Warning, "TestGameComponent: %s has no rectangle collider component", parentGameObject.lock()->getGameObjectName());
		return;
	}

	//finds what the current trigger is called
	if (rectCollider->currentTriggerTag == "Key")
	{
		//if the trigger is a key
		auto owningScene = parentGameObject.lock()->getOwningScene();
		//delete the key and door
		owningScene->deleteGameObject(owningScene->findGameObjectInSceneWithName("Key"));
		owningScene->deleteGameObject(owningScene->findGameObjectInSceneWithName("Door"));
	}
	else if (rectCollider->currentTriggerTag == "Portal")
	{
		//if touching portal reload the level (could make an end screen if i have time)
		auto owningScene = parentGameObject.lock()->getOwningScene();
		owningScene->loadScene(owningScene->getSceneName());
	}
	else if (rectCollider->currentTriggerTag == "Enemy")
	{
		//if touching and enemy reload the level
		auto owningScene = parentGameObject.lock()->getOwningScene();
		owningScene->loadScene(owningScene->getSceneName());
	}
}