#pragma once


class Object
{
public:
	float x{ 0 }, y{ 0 }, z{ 0 };
	float xl{ 1 }, yl{ 1 }, zl{ 1 };

	Object(float x, float y, float z)
		: x(x), y(y), z(z) {}
};