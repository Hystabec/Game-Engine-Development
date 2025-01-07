#pragma once
#include "Component.hpp"

class Bitmap;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Surface;

/// @brief A componet that will hold a Bitmap that allows rendering gameObject to the screen
class SpriteRenderer : public Component
{
public:
	
	SpriteRenderer(std::string fileLocation, SDL_Renderer* m_renderer, SDL_Texture* spriteTexture, SDL_Surface* spriteSurface, bool UseTransparance);
	~SpriteRenderer();

	const std::string& getFileLocation();

	virtual void init() override;
	//virtual void update() override;
	virtual void draw() override;

	virtual std::string getDataToSave() override;

private:
	std::string fileLocation = " ";
	Bitmap* sprite;
	SDL_Renderer* renderer;
};