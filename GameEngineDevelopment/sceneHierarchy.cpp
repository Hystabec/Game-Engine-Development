#include "sceneHierarchy.h"
#include "GameObject.h"

#include "Logger.h"
#include "TextureManager.h"

#include "MasterComponent.h" //this includes all componets headers
#include <SDL.h>
#include "Profiler.h"
#include <fstream>
#include "Transform.h"
#include "Game.h"

sceneHierarchy::sceneHierarchy(Game* owningGame, std::string sceneName, SDL_Renderer* gameRenderer, unsigned int customIndex)
{
	this->owningGame = owningGame;
	thisSceneName = sceneName;
	sceneRenderer = gameRenderer;

	if (customIndex != UINT_MAX)
		currentIDIndex = customIndex;
}

sceneHierarchy::~sceneHierarchy()
{
	//for each gameObject delete its parent and child refs
	//this stops two gameObjects referencing each other and stopping deletion
	// - as they are shared_ptr they can be deleted if they have refs remaining

	for (std::shared_ptr<GameObject> GOTD : hierarchy)
	{
		if(!GOTD->getIsRootNode())
			GOTD->removeParent();

		//GOTD->removeAllChildReferencesInVector();
	}

	for (std::shared_ptr<GameObject>  m_gameObject : hierarchy)
	{
		m_gameObject = nullptr;
	}

	sceneGameObjectMap.clear();

	sceneRenderer = nullptr;
}

void sceneHierarchy::changeSceneName(std::string newName)
{
	thisSceneName = newName;
}

std::string& sceneHierarchy::getSceneName()
{
	return thisSceneName;
}

unsigned int sceneHierarchy::getNumberOfGameObjects()
{
	return numberOfGameObjects;
}

std::shared_ptr<GameObject> sceneHierarchy::findGameObjectInSceneWithName(std::string gameObjectName)
{
	if (sceneGameObjectNameIDMap.count(gameObjectName))
	{
		unsigned int gameObjectID = sceneGameObjectNameIDMap[gameObjectName];

		if (std::shared_ptr<GameObject> foundGO = findGameObjectInSceneWithID(gameObjectID))
			return foundGO;
	}

	std::string tempLoggerString = ("No gameObject in " + thisSceneName + " with " + gameObjectName.c_str() + " as name\n");
	Debug_Logger->TypeDebugPrintF(Logger_Warning, tempLoggerString.c_str());

	//*boolPassCheck = false;	//this is too see if it was found outside of the function
	return nullptr;
}

std::shared_ptr<GameObject> sceneHierarchy::findGameObjectInSceneWithID(unsigned int gameObjectID)
{
	if (sceneGameObjectMap.count(gameObjectID))
	{
		std::shared_ptr<GameObject> GO = sceneGameObjectMap[gameObjectID].lock();
		return GO;
	}

	std::string tempLoggerString = ("No gameObject in " + thisSceneName + " with " + std::to_string(gameObjectID) + " as ID\n");
	Debug_Logger->TypeDebugPrintF(Logger_Warning, tempLoggerString.c_str());

	//*boolPassCheck = false;	//this is too see if it was found outside of the function
	return nullptr;
}

bool sceneHierarchy::isGameObjectNameTaken(std::string nameToCheck)
{
	bool returnVal = false;

	if (sceneGameObjectNameIDMap[nameToCheck])
		returnVal = true;

	return returnVal;
}

void sceneHierarchy::updateNameIDMap(std::shared_ptr<GameObject> gameObject, std::string oldName)
{
	sceneGameObjectNameIDMap[gameObject->getGameObjectName()] = gameObject->getGameObjectID();
	sceneGameObjectNameIDMap.erase(oldName);
}

void sceneHierarchy::deleteGameObject(std::shared_ptr<GameObject> gameObjectToDelete)
{
	if(gameObjectToDelete != nullptr)
		VectorOfGameObjectsToDelete.push_back(gameObjectToDelete);
}

std::shared_ptr<GameObject> sceneHierarchy::CreateNewGameObject(std::string gameObjectname, int xCoordinate, int ycoordinate, unsigned int customID)
{
	//std::shared_ptr<GameObject> temp_newGameObject = std::make_shared<GameObject>(GameObject(gameObjectname, xCoordinate, ycoordinate, temp_newGameObjectID));

	unsigned int indexToUse;
	bool customUsed = false;
	if (customID == UINT_MAX)
	{
		indexToUse = currentIDIndex;
	}
	else
	{
		indexToUse = customID;
		customUsed = true;
	}

	GameObject* temp = new GameObject(this, gameObjectname, indexToUse, xCoordinate, ycoordinate);
	std::shared_ptr<GameObject> tempSharGO = std::make_shared<GameObject>(*temp);
	sceneGameObjectMap[indexToUse] = tempSharGO;
	sceneGameObjectNameIDMap[gameObjectname] = indexToUse;
	hierarchy.push_back(tempSharGO);

	if(!customUsed)
		currentIDIndex++;

	numberOfGameObjects++;
	return hierarchy[hierarchy.size()-1];
}

void sceneHierarchy::AddSpriteRenderer(std::string gameObjectName, std::string spriteTextureFileExtention, std::string spriteSurfaceFileExtention, bool useTransparency)
{
	std::shared_ptr<GameObject> tempGO = findGameObjectInSceneWithName(gameObjectName);

	if (tempGO == nullptr)
	{
		Debug_Logger->TypeDebugPrintF(Logger_Error, "couldn't add sprite renderer, GameObject %s not found\n", gameObjectName.c_str());
	}
	else
	{
		tempGO->addComponent(new SpriteRenderer(spriteTextureFileExtention, sceneRenderer, Texture_Manager->Load(spriteTextureFileExtention, SDL_TRUE, sceneRenderer), SDL_LoadBMP(spriteSurfaceFileExtention.c_str()), useTransparency));
	}
}

void sceneHierarchy::AddSpriteRenderer(std::shared_ptr<GameObject> gameObject, std::string spriteTextureFileExtention, std::string spriteSurfaceFileExtention, bool useTransparency)
{
	gameObject->addComponent(new SpriteRenderer(spriteTextureFileExtention, sceneRenderer, Texture_Manager->Load(spriteTextureFileExtention, SDL_TRUE, sceneRenderer), SDL_LoadBMP(spriteSurfaceFileExtention.c_str()), useTransparency));
}

void sceneHierarchy::AddBasicMovementModule(std::string gameObjectName, int speedToMove)
{
	std::shared_ptr<GameObject> tempGO = findGameObjectInSceneWithName(gameObjectName);


	if (tempGO == nullptr)
	{
		Debug_Logger->TypeDebugPrintF(Logger_Error, "couldn't add sprite renderer, GameObject %s not found\n", gameObjectName.c_str());
	}
	else
	{
		if(speedToMove == NULL)
			tempGO->addComponent(new BasicMovementModule());
		else
			tempGO->addComponent(new BasicMovementModule(speedToMove));
	}
}

void sceneHierarchy::AddBasicMovementModule(std::shared_ptr<GameObject> gameObject, int speedToMove)
{
	if(speedToMove == NULL) 
		gameObject->addComponent(new BasicMovementModule());
	else
		gameObject->addComponent(new BasicMovementModule(speedToMove));
}

void sceneHierarchy::AddRectangleCollider(std::string gameObjectName, bool isTrigger, std::string Tag)
{
	std::shared_ptr<GameObject> tempGO = findGameObjectInSceneWithName(gameObjectName);

	if (tempGO == nullptr)
		Debug_Logger->TypeDebugPrintF(Logger_Error, "couldn't add Rectangle collider, GameObject %s not found\n", gameObjectName.c_str());
	else
		tempGO->addComponent(new RectangleCollider(isTrigger, Tag));
}

void sceneHierarchy::AddRectangleCollider(std::shared_ptr<GameObject> gameObject, bool isTrigger, std::string Tag)
{
	gameObject->addComponent(new RectangleCollider(isTrigger, Tag));
}

void sceneHierarchy::AddTestGameComp(std::string gameObjectName)
{
	std::shared_ptr<GameObject> tempGO = findGameObjectInSceneWithName(gameObjectName);

	if (tempGO == nullptr)
		Debug_Logger->TypeDebugPrintF(Logger_Error, "couldn't add Test Game comp, GameObject %s not found\n", gameObjectName.c_str());
	else
		tempGO->addComponent(new TestGameComponent());
}

void sceneHierarchy::AddTestGameComp(std::shared_ptr<GameObject> gameObject)
{
	gameObject->addComponent(new TestGameComponent());
}

void sceneHierarchy::AddBasicFollowAi(std::string gameObjectName, vec2 patrolPointOne, vec2 patrolPointTwo, int movementSpeed)
{
	std::shared_ptr<GameObject> tempGO = findGameObjectInSceneWithName(gameObjectName);

	if (tempGO == nullptr)
		Debug_Logger->TypeDebugPrintF(Logger_Error, "couldn't add Test Game comp, GameObject %s not found\n", gameObjectName.c_str());
	else
		tempGO->addComponent(new BasicFollowAI(patrolPointOne, patrolPointTwo, movementSpeed));
}

void sceneHierarchy::AddBasicFollowAi(std::shared_ptr<GameObject> gameObject, vec2 patrolPointOne, vec2 patrolPointTwo, int movementSpeed)
{
	gameObject->addComponent(new BasicFollowAI(patrolPointOne, patrolPointTwo, movementSpeed));
}

void sceneHierarchy::initScene()
{
	for (std::shared_ptr<GameObject> gameObject : hierarchy)
	{
		gameObject->initialize();
	}
	sceneHasBeenIntialized = true;
}

bool sceneHierarchy::getInitializedStatus()
{
	return sceneHasBeenIntialized;
}

void sceneHierarchy::setInitializedStatus(bool newInitializedStatus)
{
	sceneHasBeenIntialized = newInitializedStatus;
}

void sceneHierarchy::updateScene()
{
	profiler->startProfile("ActiveScene Update");
	for (std::shared_ptr<GameObject> gameObject : hierarchy)
	{
		gameObject->update();
	}
	profiler->endProfile("ActiveScene Update");
}

void sceneHierarchy::drawScene()
{
	profiler->startProfile("ActiveScene Draw");
	for (std::shared_ptr<GameObject> gameObject : hierarchy)
	{
		gameObject->draw();
	}
	profiler->endProfile("ActiveScene Draw");
}

void sceneHierarchy::cleanUpScene()
{
	deleteStoreGameObjects();
}

SDL_Renderer* sceneHierarchy::getPointerToRenderer()
{
	return sceneRenderer;
}

const std::vector<std::shared_ptr<GameObject>>& sceneHierarchy::getHierarchy()
{
	return hierarchy;
}

void sceneHierarchy::loadScene(std::string sceneNameToLoad)
{
	owningGame->loadActiveScene(sceneNameToLoad);
}

void sceneHierarchy::saveScene()
{
	owningGame->saveActiveScene();
}

void sceneHierarchy::saveCurrentSceneBackEnd(std::string saveLocation)
{
	//file layout:
	// # - means is about the scene
	// @ - means it is about gameObjects
	// > - means it is about componets
	// ~ - means it is about gameObject relations
	// | - means it is the end of a line

	std::string fileName = saveLocation + thisSceneName + ".SCDAT";
	std::ofstream outFileStream;
	outFileStream.open(fileName, std::fstream::trunc);

	outFileStream << "#" << thisSceneName << "=" << currentIDIndex << "|\n";

	//this will list all the gameObject is the hierarchy in the order that they appear
	for (std::shared_ptr<GameObject> GO : hierarchy)
	{
		outFileStream << "@" << GO->getGameObjectName() << "=" << GO->getGameObjectID() << "=" << GO->getTransform().Position().x << "=" << GO->getTransform().Position().y << "|\n";
	}

	//this will list of componets for each gameObject and anydata they need
	for (std::shared_ptr<GameObject> GO : hierarchy)
	{
		for (Component* comp : GO->getTotalComponentsVector())
		{
			outFileStream << ">" << GO->getGameObjectID() << "=" << comp->getComponentName() << "=" << comp->getDataToSave() << "|\n";
		}
	}

	//this will list of any gameObject that has a parent
	for (std::shared_ptr<GameObject> GO : hierarchy)
	{
		std::string parentString;
		if (!GO->getIsRootNode())
		{
			parentString = std::to_string(GO->getParentGameObject()->getGameObjectID());
			outFileStream << "~" << GO->getGameObjectID() << "=" << parentString << "|\n";
		}
	}

	outFileStream.close();
}

void sceneHierarchy::deleteStoreGameObjects()
{
	//loop through the gameObjects to delete vector
	for (std::weak_ptr<GameObject> GOTD : VectorOfGameObjectsToDelete)
	{
		//loops through the scene hierarchy and will remove all the game objects that are being deleted from the hierachy
		bool GOFound = false;
		int GOIndex = 0;

		for (int index = 0; index<hierarchy.size(); index++)	//inefficient as scene hierachy could be really large
		{
			if (hierarchy[index] == GOTD.lock())
			{
				GOFound = true;
				GOIndex = index;
			}
		}

		if (GOFound)
		{
			//removes parerent then removes the parent of all the children - the deletes it removes it from the scene hierarchy
			GOTD.lock()->removeParent();
			GOTD.lock()->removeAllChildReferencesInVector();
			sceneGameObjectMap.erase(GOTD.lock()->getGameObjectID());
			hierarchy.erase(hierarchy.begin() + GOIndex);	//this will delete the gameObject 
		}
		
		numberOfGameObjects--;
		//GOTD->deleteGameObject();
		//as this is its final refrence then the gameObject is automaticly deleted
	}

	VectorOfGameObjectsToDelete.clear();
}
