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

DVector2::DVector2()
{
}

DVector2::DVector2(const DVector2 &)
{
}

DVector2::DVector2(FLOAT, FLOAT)
{
}

void DVector2::Magnitude()
{
}

float DVector2::SqrMagnitude()
{
	return 0.0f;
}

void DVector2::Normalize()
{
}

void DVector2::Normalize(DVector2 &)
{
}

void DVector2::Scale(const DVector2 &)
{
}

FLOAT DVector2::operator[](int) const
{
	return FLOAT();
}

bool DVector2::operator==(const DVector2 &) const
{
	return false;
}

bool DVector2::operator!=(const DVector2 &) const
{
	return false;
}

DVector2 DVector2::operator+(const DVector2 &) const
{
	return DVector2();
}

DVector2 DVector2::operator-(const DVector2 &) const
{
	return DVector2();
}

DVector2 DVector2::operator*(FLOAT) const
{
	return DVector2();
}

DVector2 DVector2::operator/(FLOAT) const
{
	return DVector2();
}

DVector2 & DVector2::operator+=(const DVector2 &)
{
	// TODO: 在此处插入 return 语句
}

DVector2 & DVector2::operator-=(const DVector2 &)
{
	// TODO: 在此处插入 return 语句
}

DVector2 & DVector2::operator*=(FLOAT)
{
	// TODO: 在此处插入 return 语句
}

DVector2 & DVector2::operator/=(FLOAT)
{
	// TODO: 在此处插入 return 语句
}

DVector2 DVector2::Lerp(const DVector2 &, const DVector2 &, float)
{
	return DVector2();
}

DVector2 DVector2::Reflect(const DVector2 &, const DVector2 &)
{
	return DVector2();
}

float DVector2::Angle(const DVector2 &, const DVector2 &)
{
	return 0.0f;
}

float DVector2::Dot(const DVector2 &, const DVector2 &)
{
	return 0.0f;
}

float DVector2::Distance(const DVector2 &, const DVector2 &)
{
	return 0.0f;
}

float DVector2::Max(const DVector2 &, const DVector2 &)
{
	return 0.0f;
}

float DVector2::Min(const DVector2 &, const DVector2 &)
{
	return 0.0f;
}

DVector2 DVector2::Scale(const DVector2 &, const DVector2 &)
{
	return DVector2();
}

DVector2 operator*(FLOAT, const DVector2 &)
{
	return DVector2();
}

DVector3::DVector3()
{
}

DVector3::DVector3(const DVector3 &)
{
}

DVector3::DVector3(FLOAT, FLOAT, FLOAT)
{
}
