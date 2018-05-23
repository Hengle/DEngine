#pragma once
#include <math.h>
#include <d3dx10math.h>

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
	DVector2(FLOAT, FLOAT);
	void Magnitude();
	
public:
	FLOAT x, y;
} DVector2, *LPDVector2;

typedef struct DVector3
{
public:
	DVector3();
	DVector3(FLOAT, FLOAT, FLOAT);

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

class DMath
{
public :
	static float Clamp();
};
