#include "DMath.h"

DRect::DRect()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

DRect::DRect(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}