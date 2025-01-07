#pragma once
#include <string>

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

struct vector2;

/// @brief This is a class that will be able to hold/create SDL textures and will allow rendeing them to the screen - this class is housed inside the sprite renderer component
class Bitmap
{
private:
	SDL_Surface* m_pbitmapSurface;
	SDL_Texture* m_pbitmapTexture;
	SDL_Renderer* m_prenderer;
	int m_x, m_y;

public:
	/// @brief The Constructer will take data given to it by the sprite renderer and will create and sdl suraface and texture for that image
	Bitmap(SDL_Renderer* renderer, SDL_Texture* spriteTexture, SDL_Surface* spriteSurface,int xpos, int ypos, bool useTransparency = false);
	~Bitmap();

	/// @brief This function allows the rendering of the bitmap
	void drawBitmap();

	/// @brief This function allows for renderering the bitmap at a certain postion
	/// @param posX - x world position
	/// @param posY - y world position
	void drawAtPos(int posX, int posY);

	/// @brief This allows changing of the stored x value 
	/// @param m_xToSet - will change the stored x value to the passed in value
	void setm_x(int m_xToSet);

	/// @brief This allows chaging of the stored y value
	/// @param m_yToSet - will change the stored y value to the passed in value
	void setm_y(int m_yToSet);

	/// @return vector2 - This returns the width and height of the image in the form of a vector2
	vector2 getWidthHeight();
};