#pragma once
#include <minwindef.h>
#include <math.h>

#define EPSINON 0.000001f
#define RAD_TO_DEG 57.29577951308f
#define DEG_TO_RAD 0.01745329252f

#define IS_FLOAT_EQUAL(a, b) (a>=b-EPSINON&&a<=b+EPSINON)

typedef struct DRect 
{
public:
	DRect();
	DRect(FLOAT, FLOAT, FLOAT, FLOAT);

public:
	FLOAT x, y, width, height;
} DRect, *LPDRect;

typedef struct DVector2
{
public:
	DVector2();
	DVector2(const DVector2&);
	DVector2(FLOAT, FLOAT);

	float Magnitude();
	float SqrMagnitude();
	void Normalize();
	void Normalize(DVector2&);
	void Scale(const DVector2&);

	FLOAT operator [](int) const;
	bool operator ==(const DVector2&) const;
	bool operator !=(const DVector2&) const;
	DVector2 operator +(const DVector2&) const;
	DVector2 operator-(const DVector2&) const;
	DVector2 operator*(FLOAT) const;
	DVector2 operator/(FLOAT) const;
	
	friend DVector2 operator * (FLOAT, const DVector2&);

	DVector2& operator += (const DVector2&);
	DVector2& operator -= (const DVector2&);
	DVector2& operator *= (FLOAT);
	DVector2& operator /= (FLOAT);

	static DVector2 Lerp(const DVector2&, const DVector2&, float);
	static DVector2 Reflect(const DVector2&, const DVector2&);
	static float Angle(const DVector2&, const DVector2&);
	static float Dot(const DVector2&, const DVector2&);
	static float Distance(const DVector2&, const DVector2&);
	static float Max(const DVector2&, const DVector2&);
	static float Min(const DVector2&, const DVector2&);
	static DVector2 Scale(const DVector2&, const DVector2&);

	
public:
	FLOAT x, y;
} DVector2, *LPDVector2;

typedef struct DVector3
{
public:
	DVector3();
	DVector3(const DVector3&);
	DVector3(FLOAT, FLOAT, FLOAT);

	float Magnitude();
	float SqrMagnitude();
	void Normalize();
	void Normalize(DVector3&);
	FLOAT operator [](int) const;
	bool operator ==(const DVector3&) const;
	bool operator !=(const DVector3&) const;
	DVector3 operator +(const DVector3&) const;
	DVector3 operator-(const DVector3&) const;
	DVector3 operator*(FLOAT) const;
	DVector3 operator/(FLOAT) const;

	friend DVector3 operator * (FLOAT, const DVector3&);

	DVector3& operator += (const DVector3&);
	DVector3& operator -= (const DVector3&);
	DVector3& operator *= (FLOAT);
	DVector3& operator /= (FLOAT);

	static DVector3 Lerp(const DVector3&, const DVector3&, float);
	static DVector3 SLerp(const DVector3&, const DVector3&, float);
	static DVector3 Reflect(const DVector3&, const DVector3&);
	static float Angle(const DVector3&, const DVector3&);
	static float Dot(const DVector3&, const DVector3&);
	static float Distance(const DVector3&, const DVector3&);
	static float Max(const DVector3&, const DVector3&);
	static float Min(const DVector3&, const DVector3&);
	static DVector3 Scale(const DVector3&, const DVector3&);
	static DVector3 Cross(const DVector3&, const DVector3&);

public:
	FLOAT x, y, z;
} DVector3, *LPDVector3;

typedef struct DVector4
{
public:
	DVector4();
	DVector4(FLOAT, FLOAT, FLOAT, FLOAT);

public:
	FLOAT x, y, z, w;
} DVector4, *LPDVector4;

typedef struct DMatrix4x4
{
public:
	DMatrix4x4();
} DMatrix4x4;

typedef struct DQuaterion
{

} DQuaterion;

class DMath
{
public :
	static float Clamp();
};
