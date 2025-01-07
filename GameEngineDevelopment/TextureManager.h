#pragma once
#include <map>
//#include <SDL.h>
#include <string>

struct SDL_Texture;
struct SDL_Renderer;

//#define so that it is quicker the use the TextureManager instance
#define Texture_Manager TextureManager::instance()

/// @brief This is a singleton that will be responsable for holding all the tectures loaded into the game - saves memory by only loading them in on place
class TextureManager
{
private:
	std::map<std::string, SDL_Texture*> m_textureMap;
	static TextureManager* _instance;
public:

	/// @return returns a pointer to the singleton instance
	static TextureManager* instance();

	TextureManager();
	~TextureManager();

	/// @brief This function should be used when wanting to load a new file into the game - as if it is already loaded it will return a pointer to the already exsisting version
	/// @param fileName - the name of the file to load
	/// @param useTransparancy - should transparencey be used on the bitmap
	/// @param PRenderer - the renderer to load it into 
	/// @return SDL_Texture* - returns a pointer to the loaded texture
	SDL_Texture* Load(const std::string fileName, bool useTransparancy, SDL_Renderer* PRenderer);

	/// @brief This will unload a texture from memory - should be used to remove unwanted texture instead of deleting them else where
	/// @param fileName - the file name of the texture to unload
	void Unload(const std::string fileName);

	/// @brief This will check if a texture has been loaded with that file name
	/// @param fileExtention - the file name to check
	/// @param foundBool - a pointer to a bool that will either changed to true or false if it is found or not
	/// @return SDL_Texture* - returns the texture if one is found
	SDL_Texture* getTextureRefWithLocation(std::string fileExtention, bool* foundBool = nullptr);
};