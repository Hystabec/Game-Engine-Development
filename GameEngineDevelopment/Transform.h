#pragma once
#include "Vector.h"

/// @brief A basic struct to hold a position and scale vec2
struct Transform
{
private:
	vec2 position;
	vec2 scale;
public:
	Transform(vec2 positionVector)
	{
		position = positionVector;
		scale = vec2(1);
	}

	Transform(vec2 positionVector, vec2 scaleVector)
	{
		position = positionVector;
		scale = scaleVector;
	}

	vec2& Position()
	{
		return position;
	}

	vec2& Scale()
	{
		return scale;
	}
};