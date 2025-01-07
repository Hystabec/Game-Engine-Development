#include "GameObject.h"
#include <memory>
#include "Logger.h"

#include "Component.hpp"	//included here to remove circular dependecies
#include "Transform.h"
#include "sceneHierarchy.h"
#include "Profiler.h"

void GameObject::setParentSub(std::shared_ptr<GameObject> newParent)
{
	parentGameObject = newParent;
	convertTransformToLocalTransform(&newParent->getMorphingTransform());
	setActiveStatus(newParent->getActiveStatus());
	updateLocalTransforms(newParent);
	setIsRootNode(false);
}

void GameObject::setChildSub(std::shared_ptr<GameObject> newChild)
{
	this->childGameObjects.push_back(newChild);
	newChild->convertTransformToLocalTransform(&(shared_from_this()->getMorphingTransform()));
	newChild->setActiveStatus(isActive);
	updateLocalTransforms(newChild);
	newChild->setDepth(getDepth() + 1);
}


void GameObject::RemoveAllChildren()
{
	//this is mainly to be used when deleting a gameObject
	for (std::shared_ptr<GameObject> CGO : childGameObjects)
	{
		CGO->removeParent();
	}

	childGameObjects.empty();
}

void GameObject::updateLocalTransforms(std::shared_ptr<GameObject> parentGameObject)
{
	this->recalculateLocalTransforms(&parentGameObject->getTransform());
	for (std::shared_ptr<GameObject> GO : childGameObjects)
	{
		GO->updateLocalTransforms(shared_from_this());
	}
}

void GameObject::recalculateLocalTransforms(Transform* parentTransform)
{
	localTransform->Position() = vec2(
		this->transform->Position().x - parentTransform->Position().x,
		this->transform->Position().y - parentTransform->Position().y
	);

	//not sure if this is how local scale is calculated
	localTransform->Scale() = vec2(
		parentTransform->Scale().x + this->transform->Scale().x,
		parentTransform->Scale().y + this->transform->Scale().y
	);
}

void GameObject::convertTransformToLocalTransform(Transform* parentTransform)
{
	//this will be used by the child object when adding child to parent to make its transform relative to the parent gameObject
	if (isinLocal)
		return;

	//localTransform->Position() = vec2(
	//	parentTransform->Position().x - this->transform->Position().x,
	//	parentTransform->Position().y - this->transform->Position().y
	//);

	localTransform->Position() = vec2(
		this->transform->Position().x - parentTransform->Position().x,
		this->transform->Position().y - parentTransform->Position().y
	);

	//not sure if this is how local scale is calculated
	localTransform->Scale() = vec2(
		parentTransform->Scale().x + this->transform->Scale().x,
		parentTransform->Scale().y + this->transform->Scale().y
	);

	isinLocal = true;
}

void GameObject::convertLocalTrasformToTransform()
{
	//this will be used when detaching child object from parent
	//it removes the local position, so it no longer will be ofset by the local position
	if (!isinLocal)
		return;

	localTransform->Position() = vec2();
	localTransform->Scale() = vec2();

	isinLocal = false;
}

GameObject::GameObject(sceneHierarchy* scene, std::string name, int id ,int xTransform, int yTransform)
{
	this->transform = new Transform(vec2(xTransform, yTransform));
	rectBounds = rectangleBounds(transform->Position());
	localTransform = new Transform(vec2(0), vec2(1));
	thisGameObjectName = name;
	thisGameObjectID = id;
	PosessiveScene = scene;
}

GameObject::~GameObject()
{
	//Debug_Logger->TypeDebugPrintF(Logger_Warning, "gameObject %s has been deleted\n", thisGameObjectName.c_str());

	for (Component* C : totalComponents)
	{
		delete C;
		C = NULL;
	}

	for (std::shared_ptr<GameObject> childGO : childGameObjects)
	{
		childGO = nullptr;
	}

	childGameObjects.clear();
	totalComponents.clear();
	logicComponets.clear();
	graphicComponets.clear();
	PosessiveScene = nullptr;
	parentGameObject = nullptr;

	delete transform;			
	delete localTransform;
}

void GameObject::removeAllChildReferencesInVector()
{
	//this as to be public as the sceneHierarchy needs to call it - this would probably not need to be called in normal use of the program except when deleting

	RemoveAllChildren();
}

void GameObject::deleteComponent(Component* C)
{
	if (C->getComponentType().isLogicComponet)
	{
		for (int index = 0; index < logicComponets.size(); index++)
		{
			if (logicComponets[index] == C)
			{
				logicComponets.erase(logicComponets.begin() + index);
				break;
			}
		}
		numberOfLogicComponets--;
	}

	if (C->getComponentType().isGraphicsComponet)
	{
		for (int index = 0; index < graphicComponets.size(); index++)
		{
			if (graphicComponets[index] == C)
			{
				graphicComponets.erase(graphicComponets.begin() + index);
				break;
			}
		}
		numberOfGraphicsComponets--;
	}

	for (int index = 0; index < totalComponents.size(); index++)
	{
		if (totalComponents[index] == C)
		{
			totalComponents.erase(totalComponents.begin() + index);
			break;
		}
	}
	TotalNumberOfComponents--;
}

sceneHierarchy* GameObject::getOwningScene()
{
	return PosessiveScene;
}

const std::string& GameObject::getGameObjectName()
{
	return thisGameObjectName;
}

const unsigned int GameObject::getGameObjectID()
{
	if (thisGameObjectID == 0)
	{
		Debug_Logger->TypeDebugPrintF(Logger_Error, "GameObject %s has no ID\n", thisGameObjectName.c_str());
		return 0;
	}
	else
		return thisGameObjectID;
}

void GameObject::addComponent(Component* C)
{
	if (C->getComponentType().isLogicComponet)
	{
		numberOfLogicComponets++;
		logicComponets.push_back(C);
	}
	
	if (C->getComponentType().isGraphicsComponet)
	{
		numberOfGraphicsComponets++;
		graphicComponets.push_back(C);
	}

	totalComponents.push_back(C);

	C->parentGameObject = shared_from_this();
	TotalNumberOfComponents++;
}

const unsigned int& GameObject::getTotalNumberOfComponents()
{
	return TotalNumberOfComponents;
}

const std::vector<Component*>& GameObject::getTotalComponentsVector()
{
	return totalComponents;
}

const unsigned int& GameObject::getNumberOfGraphicalComponents()
{
	return numberOfGraphicsComponets;
}

const std::vector<Component*>& GameObject::getGraphicalComponentsVector()
{
	return graphicComponets;
}

const Component* GameObject::findComponent(std::string componentToFind)
{
	for (Component* comp : totalComponents)
	{
		if (comp->getComponentName() == componentToFind)
			return comp;
	}

	return nullptr;
}

const unsigned int& GameObject::getNumberOfLogicComponents()
{
	return numberOfLogicComponets;
}

const std::vector<Component*>& GameObject::getLogicComponentsVector()
{
	return logicComponets;
}

bool GameObject::getActiveStatus()
{
	return isActive;
}

void GameObject::setActiveStatus(bool setActive)
{
	//set self to passed in value then sets all children to passed in value
	isActive = setActive;
	for (std::shared_ptr<GameObject> CGO : childGameObjects)
	{
		CGO->setActiveStatus(setActive);
	}
}

/*void GameObject::addComponentSpriteRenderer(std::string spriteTextureFileExtention, std::string spriteSurfaceFileExtention)
{
	this->PosessiveScene->AddSpriteRenderer(std::shared_ptr<GameObject>(this), spriteTextureFileExtention, spriteSurfaceFileExtention);
}*/

void GameObject::initialize()
{
	for (Component* C : totalComponents)
	{
		C->initialize();
	}

}

void GameObject::update()
{
	if (!isActive)
		return;

	if (!isRootNode)
	{
		transform->Position() = parentGameObject->transform->Position() + localTransform->Position();
	}

	for (Component *C : logicComponets)
	{
		if(C->getActiveStatus())
			C->update();
	}
}

void GameObject::draw()
{
	if (!isActive)
		return;

	for (Component* C : graphicComponets)
	{
		if(C->getActiveStatus())
			C->draw();
	}
}

void GameObject::updateRectBounds()
{
	rectBounds.moveOrigin(transform->Position());
}

bool GameObject::setParent(std::shared_ptr<GameObject> parent)
{
	if (isRootNode) //checks if it already has a parent
	{
		//if not
		parentGameObject = parent;
		parent->setChildSub(shared_from_this());
		setIsRootNode(false);
		return true;
	}
	else if (!isRootNode)
	{
		parentGameObject->RemoveChildWithRef(shared_from_this());
		parentGameObject = parent;
		parent->setChildSub(shared_from_this());
		setIsRootNode(false);
		return true;
	}
	else
		return false;
}

std::shared_ptr<GameObject> GameObject::getParentGameObject()
{
	std::shared_ptr<GameObject> returnGO = nullptr;

	if (!isRootNode)
	{
		returnGO = parentGameObject;
		//return nullptr;
	}
	else
	{
		Debug_Logger->TypeDebugPrintF(Logger_Error, "gameObject | %s | doesnt have parent object\n", thisGameObjectName.c_str());
		//return nullptr;
	}

	return returnGO;
}

bool GameObject::removeParent()
{
	//if has parent GO remove else do nothing
	if (parentGameObject)
	{
		parentGameObject->RemoveChildWithRef(shared_from_this());
		parentGameObject = nullptr;
		isRootNode = true;
		Node_depth = 0;
		convertLocalTrasformToTransform();
		return true;
	}
	
	Debug_Logger->TypeDebugPrintF(Logger_Warning, "%s has no parent object\n", thisGameObjectName.c_str());
	return false;
}

bool GameObject::isFreeOfParentLooping(std::shared_ptr<GameObject> childToCheck)
{
	std::shared_ptr<GameObject> nodeToCheck = shared_from_this();

	//this will need to loop through to check child isnt thisGameObject root parent node 
	for (int i = 0; i < getDepth(); i++)
	{
		if (nodeToCheck->getParentGameObject()->getIsRootNode()) //if the parent is the root node 
		{
			//isnt in loop so true
			return true;
		}
		else if (nodeToCheck->getParentGameObject() == childToCheck)
		{
			//
			Debug_Logger->TypeDebugPrintF(Logger_Warning, "child cant be the root node of a child gameObject\n");
			return false;
		}
		else
		{
			nodeToCheck = nodeToCheck->getParentGameObject();
		}
	}

	if (Node_depth == 0)
		return true;
	else
	{
		Debug_Logger->TypeDebugPrintF(Logger_Warning, "Root node could not be found for %s\n", this->thisGameObjectName.c_str());
		return false;
	}
}

bool GameObject::AddChild(std::shared_ptr<GameObject> child, int* id)
{
	if (child == shared_from_this())	//checking its not trying to add to itself
	{
		Debug_Logger->TypeDebugPrintF(Logger_Warning, "Parent cant be itself\n");
		return false;
	}

	//checking if the child has a parent value
	if (!child->getIsRootNode())	//the child has a parent gameobject
	{
		if (isFreeOfParentLooping(child))
		{
			child->getParentGameObject()->RemoveChildWithRef(child);
			this->childGameObjects.push_back(child);
			child->setParentSub(shared_from_this());
			child->setDepth(getDepth() + 1);
			return true;
		}
		else
			return false;
	}
	else      //the child doesnt have a parent gameobject
	{
		if (isFreeOfParentLooping(child))
		{
			this->childGameObjects.push_back(child);
			child->setParentSub(shared_from_this());
			child->setDepth(getDepth() + 1);
			return true;
		}
		else
			return false;
	}

	Debug_Logger->TypeDebugPrintF(Logger_Error, "ERROR: addchild getrootnode failed\n");
	return false;
}

bool GameObject::RemoveChildWithID(int id)
{
	if (childGameObjects.size())
	{
		childGameObjects.erase(childGameObjects.begin() + id);
		return true;
	}
	else
	{
		Debug_Logger->TypeDebugPrintF(Logger_Warning, "cannot remove child at id %i in %s\n", id, this->thisGameObjectName.c_str());
	}
}

bool GameObject::RemoveChildWithRef(std::shared_ptr<GameObject> GameObjectToRemove)
{
	std::vector<std::shared_ptr<GameObject>>::iterator INit;

	INit = std::find(childGameObjects.begin(), childGameObjects.end(), GameObjectToRemove);

	if (INit != childGameObjects.end())
	{
		RemoveChildWithID(INit - childGameObjects.begin());
	}
	else
	{
		Debug_Logger->TypeDebugPrintF(Logger_Warning, "Node not found || RemoveChildWithRef()\n");
		return false;
	}	
}

std::shared_ptr<GameObject> GameObject::getChild(int id)
{
	return childGameObjects[id];
}

const std::vector<std::shared_ptr<GameObject>>& GameObject::getChildrenVector()
{
	return childGameObjects;
}

bool GameObject::getIsRootNode()
{
	return isRootNode;
}

bool GameObject::setIsRootNode(bool newRootNodeValue)
{
	isRootNode = newRootNodeValue;
	return isRootNode;
}

int GameObject::getDepth()
{
	return Node_depth;
}

void GameObject::setDepth(int newDepth)
{
	Node_depth = newDepth;
}

void GameObject::setNameOfGameObject(std::string newName)
{
	thisGameObjectName = newName;
}

Transform& GameObject::getTransform()
{
	return *transform;
}

Transform& GameObject::getMorphingTransform()
{
	//get the gameObject transform
	//if it doesnt have a parent object then it will return its normal transform
	//if it has a parent object it will return its local transform

	if (isRootNode)
	{
		return *transform;
	}
	else
	{
		return *localTransform;
	}
}

void GameObject::setAmountMovedThisFrame(vector2 movedThisFrame)
{
	amountMovedThisFrame = movedThisFrame;
}

vector2 GameObject::getAmountMovedThisFrame()
{
	return amountMovedThisFrame;
}

void GameObject::setPositionTransform(vec2 newPosition)
{
	transform->Position() = newPosition;
}

void GameObject::setMophingPossitionTransform(vec2 newPosition)
{
	//sets the gameObject transform
	//if it doesnt have a parent object then it will set its normal transform
	//if it has a parent object it will set its local transform

	if (isRootNode)
	{
		transform->Position() = newPosition;
	}
	else
	{
		localTransform->Position() = newPosition;
	}
}

void GameObject::setScaleTransform(vec2 newScale)
{
	transform->Scale() = newScale;
}

void GameObject::setMophingScaleTransform(vec2 newScale)
{
	//sets the gameObject scale
	//if it doesnt have a parent object then it will set its normal scale
	//if it has a parent object it will set its local scale

	if (isRootNode)
	{
		transform->Scale() = newScale;
	}
	else
	{
		localTransform->Scale() = newScale;
	}
}

Transform& GameObject::getLocalTransform()
{
	return *localTransform;
}

rectangleBounds GameObject::setRectBoundsWH(int width, int height)
{
	rectBounds.resize(width, height);
	return rectBounds;
}

rectangleBounds GameObject::getRectBounds()
{
	return rectBounds;
}

const rectangleBounds* GameObject::getRectBoundsPtr()
{
	return &rectBounds;
}

bool GameObject::isInBounds(vec2 pointToLookFor)
{
	//the rectBounds hasnt been set
	if (rectBounds.topLeft == vec2(0) && rectBounds.bottomRight == vec2(0))
		return false;

	updateRectBounds();

	//uses the function in the struct to check if the point is in its bounds
	return rectBounds.isPointInBounds(pointToLookFor);
}
