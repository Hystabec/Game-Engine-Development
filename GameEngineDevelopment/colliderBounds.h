#pragma once
#include "Transform.h"

/// @brief This is a struct used for the collistion system it will hold an origin point, a width and height
/// as well as other vector that will be relative from the origin position NOT world position
struct rectangleBounds
{
	vec2 origin;
	vec2 centre;
	vec2 topLeft;
	vec2 topRight;
	vec2 bottomLeft;
	vec2 bottomRight;
	int _width;
	int _height;
	bool topLeftIsOrigin;

	//default constuctor sets all values to 0
	rectangleBounds()
	{
		origin = vec2(0);
		centre = vec2(0);
		topLeft = vec2(0);
		topRight = vec2(0);
		bottomLeft = vec2(0);
		bottomRight = vec2(0);
		_height = 0;
		_width = 0;
		topLeftIsOrigin = true;
	}

	rectangleBounds(vec2 origin, bool useTopLeft = true)
	{
		this->origin = origin;
		centre = vec2(0);
		topLeft = vec2(0);
		topRight = vec2(0);
		bottomLeft = vec2(0);
		bottomRight = vec2(0);
		_height = 0;
		_width = 0;
		topLeftIsOrigin = useTopLeft;
	}

	//takes in an origin a width hight and bool for top left
	//this will calculate a retangle around the origin point using the width and hight; by default this will treat the origin by top left but can be overwritten and will treat it as the centre
	rectangleBounds(vec2 origin, int width, int height, bool useTopLeft = true)
	{
		this->origin = origin;
		this->_width = width;
		this->_height = height;

		if (useTopLeft)
		{
			this->centre = vec2(width / 2, height / 2);
			this->topLeft = origin;
			this->topRight = vec2(width, 0);
			this->bottomLeft = vec2(0, height);
			this->bottomRight = vec2(width, height);
			topLeftIsOrigin = true;
		}
		else
		{
			this->centre = origin;
			this->topLeft = vec2(-(width / 2), -(height / 2));
			this->topRight = vec2((width / 2), -(height / 2));
			this->bottomLeft = vec2(-(width / 2), (height / 2));
			this->bottomRight = vec2((width / 2), (height / 2));
			topLeftIsOrigin = false;
		}
	}

	//this will take in a vec2 and will make that the origin and will move the other points to match it
	void moveOrigin(vec2 newOriginLocation)
	{
		origin = newOriginLocation;
	}

	void resize(int newWidth, int newHeight)
	{
		_width = newWidth;
		_height = newHeight;

		if (topLeftIsOrigin)
		{
			this->centre = vec2(newWidth / 2, newHeight / 2);
			this->topRight = vec2(newWidth, 0);
			this->bottomLeft = vec2(0, newHeight);
			this->bottomRight = vec2(newWidth, newHeight);
		}
		else
		{
			this->topLeft = vec2(-(newWidth / 2), -(newHeight / 2));
			this->topRight = vec2((newWidth / 2), -(newHeight / 2));
			this->bottomLeft = vec2(-(newWidth / 2), (newHeight / 2));
			this->bottomRight = vec2((newWidth / 2), (newHeight / 2));
		}
	}

	//this will check between the corner points and see if the point to check for is between them; returns true if it is else returns false
	bool isPointInBounds(vec2 pointToCheck)
	{
		bool xArg1 = pointToCheck.x >= topLeft.x + origin.x;
		bool xArg2 = pointToCheck.x <= topRight.x + origin.x;

		bool yArg1 = pointToCheck.y >= topLeft.y + origin.y;
		bool yArg2 = pointToCheck.y <= bottomLeft.y + origin.y;

		if (xArg1 && xArg2 && yArg1 && yArg2)
			return true;
		else
			return false;
	}

	bool operator ==(const rectangleBounds& other)
	{
		if (
			origin == other.origin &&
			topLeft == other.topLeft &&
			topRight == other.topRight &&
			bottomLeft == other.bottomLeft &&
			bottomRight == other.bottomRight
			)
		{
			return true;
		}

		return false;
	}
};