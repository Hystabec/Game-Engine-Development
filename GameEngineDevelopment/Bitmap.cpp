
#include "Bitmap.h"
#include "SDL.h"
#include "SDL_render.h"
#include <string>
#include "Transform.h"

//using namespace std;  

Bitmap::Bitmap(SDL_Renderer* renderer, SDL_Texture* spriteTexture, SDL_Surface* spriteSurface,int xpos, int ypos, bool useTransparency)
{
	m_prenderer = renderer;
	m_pbitmapTexture = spriteTexture;

	m_pbitmapSurface = spriteSurface;
	if (!m_pbitmapSurface)
	{
		//bitmap not loaded, output error
		printf("SURFACE for bitmap not loaded \n");
		printf("%s\n", SDL_GetError());
	}

	//store the position vals
	m_x = xpos;
	m_y = ypos;
}

Bitmap::~Bitmap()
{
	if (m_pbitmapSurface)
		SDL_FreeSurface(m_pbitmapSurface);
}

void Bitmap::drawBitmap()
{
	//render the bitmap at the x/y coords
	if (m_pbitmapTexture)
	{
		SDL_Rect destRect = { m_x, m_y, m_pbitmapSurface->w, m_pbitmapSurface->h };
		SDL_RenderCopy(m_prenderer, m_pbitmapTexture, NULL, &destRect);
	}
}

void Bitmap::drawAtPos(int posX, int posY)
{
	if (m_pbitmapTexture)
	{
		SDL_Rect destRect = { posX, posY, m_pbitmapSurface->w, m_pbitmapSurface->h };
		SDL_RenderCopy(m_prenderer, m_pbitmapTexture, NULL, &destRect);
	}
}

void Bitmap::setm_x(int m_xToSet)
{
	m_x = m_xToSet;
}

void Bitmap::setm_y(int m_yToSet)
{
	m_y = m_yToSet;
}

vector2 Bitmap::getWidthHeight()
{
	return vec2(m_pbitmapSurface->w, m_pbitmapSurface->h);
}
