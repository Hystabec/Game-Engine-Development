#pragma once

/// @brief A basic struct to hold and x and y value in one location
struct vector2
{
	int x, y;

	vector2()
	{
		x = 0;
		y = 0;
	}

	vector2(int X)
	{
		x = X;
		y = X;
	}

	vector2(int X, int Y)
	{
		x = X;
		y = Y;
	}

	vector2 operator +(vector2 other) { return vector2(this->x + other.x, this->y + other.y); };
	vector2 operator -(vector2 other) { return vector2(this->x - other.x, this->y - other.y); };
	bool operator ==(vector2 other) { if (other.x == this->x && other.y == this->y)return true; else return false; };
};

typedef vector2 vec2;