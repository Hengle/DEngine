#pragma once
#include "d3dx10math.h"
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


class DMath
{
public :
	static float Clamp()
};
