#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Vector.h"

class GameObject;
class Game;

struct SDL_Renderer;

/// @brief This is the class that will hold all the gameObject and allows updating, drawing and manipulating them
class sceneHierarchy
{
public:
    sceneHierarchy(Game* owningGame, std::string sceneName, SDL_Renderer* gameRenderer, unsigned int customCurrentID = UINT_MAX);
    ~sceneHierarchy();

    /// @brief Changes the name of the scene
    /// @param newName - changes the scene name to the passed in value
    void changeSceneName(std::string newName);

    /// @return std::string - returns the current scene name as a string
    std::string& getSceneName();

    /// @return unsigned int - returns the number of gameObjects in the scene
    unsigned int getNumberOfGameObjects();

    /// @brief Takes a string and finds a gameObject then returns the found gameObject 
    /// @param gameObjectName - the name of the gameObject to find
    /// @return std::shared_ptr<GameObject> - returns the found gameObject
    std::shared_ptr<GameObject> findGameObjectInSceneWithName(std::string gameObjectName);

    /// @brief Takes an ID (int) and finds a gameObject then returns a ptr to the found gameObject
    /// @param gameObjectID - the Id of the object to find
    /// @return std::shared_ptr<GameObject> - returns the found gameObject
    std::shared_ptr<GameObject> findGameObjectInSceneWithID(unsigned int gameObjectID);

    /// @brief This will take a string name and will check all the gameObjects in the scene to see if the name is already taken
    /// @param nameToCheck - the name to check is taken
    /// @return bool - true if it is taken, false if it isnt taken
    bool isGameObjectNameTaken(std::string nameToCheck);

    /// @brief This will be used when chaging the name of a gameObject so that old name dont remain in the map even after they are changed
    /// @param gameObject - a ptr to the gameObject that had its name changed
    /// @param oldName - the old name of the gameObject
    void updateNameIDMap(std::shared_ptr<GameObject> gameObject, std::string oldName);

    /// @brief This will take a ptr to a gameObject and add it to a list of gameObjects that will be deleted at the end of the update loop
    /// @param gameObjectToDelete - pointer to the gameObject to delete
    void deleteGameObject(std::shared_ptr<GameObject> gameObjectToDelete);

    /// @brief This will create a new gameObject in the sceneHierarchy that will have no components
    /// @param gameObjectName - the name of the new gameObject
    /// @param xCoordinate - its start x value
    /// @param ycoordinate - its start y value
    /// @param customID - this should only be used by the load system - it will for the gameObject ID to be the passed in value
    /// @return std::shared_ptr<GameObject> - returns a pointer to the newly created gameObject
    std::shared_ptr<GameObject> CreateNewGameObject(std::string gameObjectName, int xCoordinate=0, int ycoordinate=0, unsigned int customID = UINT_MAX);

    
    /// @brief Adds a sprite renderer to a gameObject
    /// @param gameObjectName - name of the gameObject to add it too
    /// @param spriteTextureFileExtention - the filelocation to use for the texture
    /// @param spriteSurfaceFileExtention - the filelocation to use for the surface 
    /// @param useTransparency - should the rgb(255,0,255) be treated as transparnet
    void AddSpriteRenderer(std::string gameObjectName, std::string spriteTextureFileExtention, std::string spriteSurfaceFileExtention, bool useTransparency = true);

    /// @brief Adds a sprite renderer to a gameObject
    /// @param gameObject - pointer of the gameObject to add it too
    /// @param spriteTextureFileExtention - the filelocation to use for the texture
    /// @param spriteSurfaceFileExtention - the filelocation to use for the surface 
    /// @param useTransparency - should the rgb(255,0,255) be treated as transparnet
    void AddSpriteRenderer(std::shared_ptr<GameObject> gameObject, std::string spriteTextureFileExtention, std::string spriteSurfaceFileExtention, bool useTransparency = true);

    /// @brief Adds a basic movement module to a gameObject
    /// @param gameObjectName - the name of the gameObject to add it too
    /// @param speedToMove - the speed that the component should cause the gameObject to move
    void AddBasicMovementModule(std::string gameObjectName, int speedToMove = NULL);

    /// @brief Adds a basic movement module to a gameObject
    /// @param gameObject - pointer of the gameObject to add it too
    /// @param speedToMove - the speed that the component should cause the gameObject to move
    void AddBasicMovementModule(std::shared_ptr<GameObject> gameObject, int speedToMove = NULL);

    /// @brief Adds a rectangle collider to a gameObject
    /// @param gameObjectName - the name of the gameObject to add it too
    /// @param isTrigger - a bool for if it is a trigger or not
    /// @param Tag - the tag of the trigger
    void AddRectangleCollider(std::string gameObjectName, bool isTrigger = false, std::string Tag = "Default");

    /// @brief Adds a rectangle collider to a gameObject
    /// @param gameObject - pointer of the gameObject to add it too
    /// @param isTrigger - a bool for if it is a trigger or not
    /// @param Tag - the tag of the trigger
    void AddRectangleCollider(std::shared_ptr<GameObject> gameObject, bool isTrigger = false, std::string Tag = "Default");

    /// @brief Adds a Test game comp to a gameObject
    /// @param gameObjectName - the name of the gameObject to add it to
    void AddTestGameComp(std::string gameObjectName);

    /// @brief Adds a Test game comp to a gameObject
    /// @param gameObject - pointer of the gameObject to add it to
    void AddTestGameComp(std::shared_ptr<GameObject> gameObject);

    /// @brief Adds a basic AI componet to a gameObject
    /// @param gameObjectName - the name of the gameObject to add it too
    /// @param patrolPointOne - the patrol point one as a vec2
    /// @param patrolPointTwo - the patrol point two as a vec2
    /// @param movementSpeed - the speed it should move as int
    void AddBasicFollowAi(std::string gameObjectName, vec2 patrolPointOne = vec2(0), vec2 patrolPointTwo = vec2(0), int movementSpeed = 5);

    /// @brief Adds a basic AI componet to a gameObject
    /// @param gameObject - pointer of the gameObject to add it too
    /// @param patrolPointOne - the patrol point one as a vec2
    /// @param patrolPointTwo - the patrol point two as a vec2
    /// @param movementSpeed - the speed it should move as int
    void AddBasicFollowAi(std::shared_ptr<GameObject> gameObject, vec2 patrolPointOne = vec2(0), vec2 patrolPointTwo = vec2(0), int movementSpeed = 5);

    /// @brief This will loop through all the gameObjects in the scene and call initialize on it
    void initScene();

    /// @brief This will get the value of sceneHasBeenIntialized
    /// @return bool - true if scene is intialized, false if not
    bool getInitializedStatus();

    /// @brief sets the initializationStatus
    /// @param newInitializedStatus - set intializedationsatus to passed in value
    void setInitializedStatus(bool newInitializedStatus);

    /// @brief calls update on all the gameObjects in the scene
    void updateScene();

    /// @brief calls draw on all the gameObjects in the scene
    void drawScene();

    /// @brief This should only be used at the end of the update loop - it will delete all the gameObject that have been delete from the scene that update
    void cleanUpScene();

    /// @return SDL_Renderer* - returns a pointer to the render used in the scene
    SDL_Renderer* getPointerToRenderer();

    /// @return std::vector<std::shared_ptr<GameObject>> - returns a reference to the hierarchy of the scene
    const std::vector<std::shared_ptr<GameObject>>& getHierarchy();

    /// @brief this will buffer a scene to load that will be loaded at the end of the current update loop
    /// @param sceneNameToLoad - the name of the scene to load
    void loadScene(std::string sceneNameToLoad);

    /// @brief This will save the current scene in as .SCDAT file with the same name as the scene name
    void saveScene();

    /// @brief This will actually do all the writing to file and passing the data - would prefer this to be private but game needs to see it
    /// @param saveLocation - the location that the file will be saved too
    void saveCurrentSceneBackEnd(std::string saveLocation); 
private:
    std::vector<std::weak_ptr<GameObject>> VectorOfGameObjectsToDelete;
    std::map<unsigned int, std::weak_ptr<GameObject>> sceneGameObjectMap;
    std::map<std::string, unsigned int> sceneGameObjectNameIDMap;

    /// @brief this will delete all the stored gameObjects at the end of the update loop
    void deleteStoreGameObjects();
    bool sceneHasBeenIntialized = false;
    std::string thisSceneName;
    std::vector<std::shared_ptr<GameObject>> hierarchy;
    unsigned int currentIDIndex = 1;
    unsigned int numberOfGameObjects = 0;
    SDL_Renderer* sceneRenderer;
    Game* owningGame;
};