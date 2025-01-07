#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Transform.h";
#include "colliderBounds.h"

class Component;	//predefined so that it doesnt cause circular dependecies 
class Transform;
class sceneHierarchy;

class GameObject : public std::enable_shared_from_this<GameObject>
{
private:
	bool isActive = true;
	std::string thisGameObjectName;
	unsigned int thisGameObjectID;
	sceneHierarchy* PosessiveScene;

	std::vector<Component*> totalComponents;
	unsigned int TotalNumberOfComponents = 0;

	std::vector<Component*> logicComponets;
	unsigned int numberOfLogicComponets = 0;

	std::vector<Component*> graphicComponets;
	unsigned int numberOfGraphicsComponets = 0;

	bool isRootNode = true;
	unsigned int Node_depth = 0;
	std::shared_ptr<GameObject> parentGameObject = nullptr;
	std::vector<std::shared_ptr<GameObject>> childGameObjects;

	/// @brief This will set the value of IsRootNode to the passed in value
	/// @param newRootNodeValue - is a bool that will change become the new value of IsRootNode
	/// @return bool - the new value of IsRootNode (a little pointless)
	bool setIsRootNode(bool newRootNodeValue);

	/// @brief This is a function that will let you change the value of Node_Depth which says how deep the gameObject is in the parent tree
	/// @param int - the value of Node_Depth will become the passed in int value
	void setDepth(int newDepth);

	/// @brief This is a function that will check that when a gameObejct is being made a child it wont create a loop of parent -> child -> parent 
	/// @param std::shared_ptr<GameObject> childToCheck - this will take the passed in child and make sure that it wont create a loop while parenting
	/// @return bool - if there was a loop found true is returned if not false is returned
	bool isFreeOfParentLooping(std::shared_ptr<GameObject> childToCheck);

	/// @brief This is the function that handles the parenting of a new gameObject once all the checks are done
	/// @param std::shared_ptr<GameObject> newParent - will set the parent of this gameObject to the passed in gameObject
	void setParentSub(std::shared_ptr<GameObject> newParent);

	/// @brief This is the function that handles the childing of a new gameObject once all the checks are done
	/// @param std::shared_ptr<GameObject> newChild - will be added to the children vector of this game object
	void setChildSub(std::shared_ptr<GameObject> newChild);

	/// @brief This is a function that will remove all the children from the child vec - as well as remove all the childrens parents
	void RemoveAllChildren();

	/// @brief This is a function that will calculate the local transform of this gameObject based on the parent gameObject
	/// @param std::shared_ptr<GameObject> parentGameObject - The passed in value is what is used to calcualate the local transform based on its transform as the origin
	void updateLocalTransforms(std::shared_ptr<GameObject> parentGameObject);

	/// @brief This will update the values of the local transform so that they are accurate
	/// @param Transform* parentTransform - Will base this->localTransform using the passed in transform as the new origin point
	void recalculateLocalTransforms(Transform* parentTransform);

	/// @brief This will take the current transform of this object and change it into local tranform space
	/// @param Transform* parentTransform - This passed value will be treated as the new origin that local transform will be calculated from
	void convertTransformToLocalTransform(Transform* parentTransform);

	/// @brief This function will be remove the local transform and treat world 0,0 as the new origin - used when unparenting
	void convertLocalTrasformToTransform();

	/// @brief A bool that will say if the current Transform is in local space of world space
	bool isinLocal = false;


	vec2 amountMovedThisFrame = vec2();

	rectangleBounds rectBounds;
	Transform* transform;
	Transform* localTransform;
public:
	GameObject(sceneHierarchy* scene, std::string name, int id, int xTransform = 0, int yTransform = 0);	//could pass in a transform instead of two ints?
	~GameObject();

	/// @brief This will be used to remove all children from the gameObject
	void removeAllChildReferencesInVector();

	/// @brief This is used when wanting to delete a component from the gameObject
	/// @param Component* C - is a pointer the the component that want to be deleted - should be used in along side get componet so you have a pointer to the component 
	void deleteComponent(Component* C);

	/// @brief This returns a pointer to the scene that it is in
	/// @return sceneHierarchy* - This is a pointer to the scene the gameObject is in
	sceneHierarchy* getOwningScene();

	const std::string& getGameObjectName();

	const unsigned int getGameObjectID();
	/// @brief This will be used by the sceneHieracy to add a component to the gameObject which get added to the components vector
	/// @param Component* C - The passed in component will get converted to its base class and the added to the vector
	void addComponent(Component* C);

	const unsigned int& getTotalNumberOfComponents();

	/// @brief This returns a vector of all the components on the gameObject - mainly need for UI - findComponent() should be used in normal use
	const std::vector<Component*>& getTotalComponentsVector();

	/// @brief As the components are split between logical and graphical
	/// @return unsigned int for the number of logical components
	const unsigned int& getNumberOfLogicComponents();

	/// @brief This would be used if only the logical componets are wanted
	const std::vector<Component*>& getLogicComponentsVector();

	/// @brief As the components are split between logical and graphical
	/// @return unsigned int for the number of graphical components
	const unsigned int& getNumberOfGraphicalComponents();

	/// @brief This would be used if only the graphical componets are wanted
	const std::vector<Component*>& getGraphicalComponentsVector();

	/// @brief This will be used when wanting to find a specific component
	/// @param std::string componentToFind - This is the name of the component that wants to be found e.g. "Rectangle Collider"
	/// @return Component* - a pointer to the componet that was searched for
	const Component* findComponent(std::string componentToFind);

	bool getActiveStatus();
	void setActiveStatus(bool setActive);

	/// @brief This will loop through all the components on the gameObject and intialize them (if they havent already been)
	void initialize();		//this will iterate through all the components in the gameObject and run their initialize function

	/// @brief This is called each update and will call update on each component in order they were added
	void update();			//this will iterate through all the components in the gameObject and run their update function

	/// @brief This is called near the end of each update and will call draw on each component in the order the were added
	void draw();

	/// @brief This should be called before you get the rect bounds of the gameObject otherwise they could be outofdate
	void updateRectBounds();

	/// @brief This is used when wanting to changed the parent of this gameObject
	/// @param std::shared_ptr<GameObject> parent - This passed in value will become the new parent of the gameObject
	/// @return bool - to show if the parenting failed or succeded
	bool setParent(std::shared_ptr<GameObject> parent);

	/// @brief This should be used if you want to find the parent object of this gameObject
	/// @return std::shared_ptr<GameObject> - returns a pointer to the parent
	std::shared_ptr<GameObject> getParentGameObject();

	/// @brief This will remove the parent from the current gameObject
	/// @return bool - to show if the parent removal failed or succeded
	bool removeParent();

	/// @brief This will be used when trying to make the passed in gameObject a child of this gameObject
	/// @param child - will become a child of this gameObject
	/// @param id - this is no longer used (probably should have been removed)
	/// @return bool - show if the adding passed or failed
	bool AddChild(std::shared_ptr<GameObject> child, int* id = NULL);

	/// @brief This will remove a child with a specific child id
	/// @param id - the id of the gameObject to remove
	/// @return bool - show if the removal passed or failed
	bool RemoveChildWithID(int id);

	/// @brief This will remove a specific gameObject from the child vector
	/// @param GameObjectToRemove - ths is the gameObject that will be removed
	/// @return bool - show if the removal passed or failed
	bool RemoveChildWithRef(std::shared_ptr<GameObject> GameObjectToRemove);

	/// @brief This will return a pointer to a gameObject that is found with the id
	/// @param id - id of the child you want to find
	/// @return std::shared_ptr<GameObject> - pointer to the found child or nullptr if not found
	std::shared_ptr<GameObject> getChild(int id);

	/// @brief This will get a reference to the whole childVector
	/// @return returns a const ref to the child vector
	const std::vector<std::shared_ptr<GameObject>>& getChildrenVector();

	/// @brief This is used if you want to find if a gameObject has a parent
	/// @return bool - true if it has no parent, false if it does
	bool getIsRootNode();

	/// @brief This will be used if you want to get depth of the gameObject in the relation tree
	/// @return int - how deep it is in the relation tree
	int getDepth();

	/// @brief This is used if you want to rename the gameObject
	/// @param newName - the name of the gameObject will become the passed in name
	void setNameOfGameObject(std::string newName);

	/// @return returns the transform of the gameObject in world space
	Transform& getTransform();
	
	/// @return returns transfrom in local space if it has a parent, returns transfrom in world space if not
	Transform& getMorphingTransform();

	void setAmountMovedThisFrame(vector2 movedThisFrame);
	vector2 getAmountMovedThisFrame();

	/// @brief Change the position value of the gameObject in world space
	/// @param newPosition - will update position to become the passed in value
	void setPositionTransform(vec2 newPosition);

	/// @brief This will either update the position is local space if it has a parent or in world space if it doesnt
	/// @param newPosition - will become the new position in either local space or world space
	void setMophingPossitionTransform(vec2 newPosition);

	/// @brief This would update the scale (scale doesnt do anything)
	/// @param newScale - scale become the passed in value
	void setScaleTransform(vec2 newScale);

	/// @brief This would changed the scale in local space if it had a parnet or world space if it didnt (scale doesnt do anything)
	/// @param newScale - scale will become the passed in value in either local space or world space
	void setMophingScaleTransform(vec2 newScale);

	/// @return get the transfrom is specificaly local space
	Transform& getLocalTransform();

	/// @brief This will be used to change the width and height of the rectangleBounds 
	/// @param width - sets the new width
	/// @param height - sets the new height
	/// @return will return the new rectangleBounds
	rectangleBounds setRectBoundsWH(int width, int height);

	/// @return The gameObjects rectangleBounds 
	rectangleBounds getRectBounds();

	/// @return returns a const ptr to the gameObjects rectangleBounds
	const rectangleBounds* getRectBoundsPtr();

	/// @brief This will be used if you want to find if a point is within the bounds of the gameObject
	/// @param pointToLookFor - the point to check is in the bounds
	/// @return bool - true if in bounds, false if not
	bool isInBounds(vec2 pointToLookFor);

	//this isnt a great way of doing this but i am running out of time
	bool collidedOnTop = false;
	bool collidedOnLeft = false;
	bool collidedOnRight = false;
	bool collidedOnBottom = false;
};