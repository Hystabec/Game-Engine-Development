#include "SpriteRenderer.h"
#include "Bitmap.h"
#include <memory>
#include "GameObject.h"	
#include "Logger.h"
#include "Transform.h"
#include <SDL.h>

#include "EventManager.h"

SpriteRenderer::SpriteRenderer(std::string fileLocation, SDL_Renderer* m_renderer, SDL_Texture* spriteTexture, SDL_Surface* spriteSurface, bool UseTransparance)
{
	nameOfComponent = "Sprite Renderer";
	componetType = ComponetType(false, true);
	componetClass = ComponetClass::CC_spriteRenderer;

	this->fileLocation = fileLocation;
	sprite = new Bitmap(m_renderer, spriteTexture, spriteSurface, 0, 0, UseTransparance);
}

SpriteRenderer::~SpriteRenderer()
{
	delete sprite;
	sprite = nullptr;

	parentGameObject.reset();
}

const std::string& SpriteRenderer::getFileLocation()
{
	return fileLocation;
}

void SpriteRenderer::init()
{
	if (std::shared_ptr<GameObject> PGO = parentGameObject.lock())	//as this is done before add component this doesnt have a current parentGameObject
	{
		sprite->setm_x(PGO->getTransform().Position().x);
		sprite->setm_y(PGO->getTransform().Position().y);
		PGO->setRectBoundsWH(sprite->getWidthHeight().x, sprite->getWidthHeight().y);
	}
	else
	{
		Debug_Logger->TypeDebugPrintF(Logger_Warning, "parentGameObject could not lock - initialization\n");
	}
}

void SpriteRenderer::draw()
{
	if (!isActive)
		return;

	std::shared_ptr<GameObject> PGO = parentGameObject.lock();

	if (parentGameObject.expired())	//the pointer is expired
	{
		Debug_Logger->TypeDebugPrintF(Logger_Error, "pointer expired\n");
	}

	if (PGO)	//fails this statement
	{
		sprite->setm_x(PGO->getTransform().Position().x);
		sprite->setm_y(PGO->getTransform().Position().y);
	}
	else
	{
		Debug_Logger->TypeDebugPrintF(Logger_Warning, "SpriteRenderer.cpp||ParentGameObject could not lock - update()\n");
	}

	sprite->drawBitmap();
}

std::string SpriteRenderer::getDataToSave()
{
	std::string activeAsString = isActive ? "true" : "false";

	return (activeAsString + "=" + fileLocation);
}

