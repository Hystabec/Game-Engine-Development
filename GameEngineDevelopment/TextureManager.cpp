#include "TextureManager.h"

#include <SDL.h>
#include "Logger.h"

TextureManager* TextureManager::_instance = 0;

TextureManager* TextureManager::instance()
{
	if (!_instance)
	{
		_instance = new TextureManager();
	}

	return _instance;
}

TextureManager::TextureManager()
{
	
}

TextureManager::~TextureManager()
{
	for (auto value : m_textureMap)
	{
		SDL_DestroyTexture(value.second);
		value.second = nullptr;
	}
}

SDL_Texture* TextureManager::Load(const std::string fileName, bool useTransparancy, SDL_Renderer* PRenderer)
{
	SDL_Texture* m_pbitmapTexture = nullptr;
	//check map for file name existance
	auto searchresult = m_textureMap.find(fileName);
	if (searchresult != m_textureMap.end())
	{
		m_pbitmapTexture = m_textureMap[fileName];
	}
	else //not found, load, save and return
	{
		//if file does not exist in map
		SDL_Surface* pTempSurface = SDL_LoadBMP(fileName.c_str());
		if (!pTempSurface)
		{
			Debug_Logger->TypeDebugPrintF(Logger_Error, "SURFACE for bitmap '%s' not loaded\n", fileName.c_str());
			Debug_Logger->TypeDebugPrintF(Logger_Error, "%s\n", SDL_GetError());

			//printf("SURFACE for bitmap '%s' not loaded\n", fileName.c_str());
			//printf("%s\n", SDL_GetError());
		}
		else
		{
			if (useTransparancy)
			{
				Uint32 colourKey = SDL_MapRGB(pTempSurface->format, 255, 0, 255);
				SDL_SetColorKey(pTempSurface, SDL_TRUE, colourKey);
			}
			m_pbitmapTexture = SDL_CreateTextureFromSurface(PRenderer, pTempSurface);
			if (!m_pbitmapTexture)
			{
				printf("TEXTURE for bitmap '%s' not loaded!\n", fileName.c_str());
				printf("%s\n", SDL_GetError());
			}

			m_textureMap[fileName] = m_pbitmapTexture;
		}
	}
	return m_pbitmapTexture;
}

void TextureManager::Unload(const std::string fileName)
{
	auto searchResult = m_textureMap.find(fileName);
	if (searchResult != m_textureMap.end())
	{
		SDL_DestroyTexture( m_textureMap[fileName]);
		m_textureMap[fileName] = nullptr;
		m_textureMap.erase(searchResult);
	}
}

SDL_Texture* TextureManager::getTextureRefWithLocation(std::string fileExtention, bool* foundBool)
{
	//check map for file name existance
	auto searchResult = m_textureMap.find(fileExtention);
	if (searchResult != m_textureMap.end())
	{
		//was found in the map
		if (foundBool)
			*foundBool = true;

		return m_textureMap[fileExtention];
	}
	else
	{
		//wasnt found in the map
		Debug_Logger->TypeDebugPrintF(Logger_Warning, "No texture found with file extention %s \n", fileExtention);

		if (foundBool)
			*foundBool = false;

		return nullptr;
	}
}