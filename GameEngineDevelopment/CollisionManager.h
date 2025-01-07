#pragma once
#include <vector>
#include <memory>

class RectangleCollider;
struct ColliderReturn;
struct TriggerReturn;
struct rectangleBounds;
class GameObject;

#define Collision_Manager CollisionManager::instace()

/// @brief This is a singleton that will handle all the collisions and tiggers in the game - the collider componts will query this to see if the have collided or if they are in a trigger
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	/// @return returns the collisionManager singleton instance
	static CollisionManager* instace();
	
	/// @brief This will deal with pushing object out of eachother
	void update();

	/// @brief This is a function to add gameObject to the colliders to be updated
	/// @param colliderToAdd - ptr to the gameObject to be added
	void addCollider(std::weak_ptr<GameObject> colliderToAdd);

	/// @brief This is a function to remove gameObject from the collider vector
	/// @param colliderToRemove - ptr to the gameObject to remove
	void removeCollider(std::weak_ptr<GameObject> colliderToRemove);

	

	/// @brief query that will take a collider ref and will return a ColliderReturn which has a bool if it colliding with something else
	/// @param colliderToCheck - the rectbounds to check are colliding with anything
	/// @return colliderReturn - returns a struct that holds data on if it collidered with anything and if it did what at was
	ColliderReturn isColliding(rectangleBounds colliderToCheck); //Bonus - colliders could have a type - so they can only collide with certain types

	/// @brief quary that will take a rectangleBound and will return a trigger return that will hold a bool if it was in a trigger
	/// @param colliderToCheck - the rectbounds to check are in a trigger
	/// @return TriggerReturn - returns a struct that holds if it was in a trigger, what the trigger was, and the tag of the trigger
	TriggerReturn inTrigger(rectangleBounds colliderToCheck);
	//is in trigger, this will on check colliders marked as triggers

private:
	static CollisionManager* _instance;

	//holds vector of collider comps
	std::vector<std::weak_ptr<GameObject>> collidersVector;
};