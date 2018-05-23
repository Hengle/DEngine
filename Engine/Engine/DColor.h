#pragma once
 
typedef struct DColor
{
public:
	DColor();
	DColor(float, float, float, float);
	DColor(float, float, float);
	float GetGrayScale();
	void ToHSV(float&, float&, float&);

	DColor operator+(const DColor&);
	DColor operator-(const DColor&);
	DColor operator*(const DColor&);
	DColor operator*(const float&);
	DColor operator/(const DColor&);
	DColor operator/(const float&);
	bool operator==(const DColor&);
	bool operator!=(const DColor&);
	float operator[](int);

	static DColor Lerp(DColor, DColor, float);
	static DColor HSVToRGB(float, float, float);
	static void RGBToHSV(const DColor, float&, float&, float&);

private:
	static void RGBToHSV_Internal(float, float, float, float, float&, float&, float&);

public:
	float r, g, b, a;

	
} DColor;

const DColor dcol_red        = DColor(1.0f, 0.0f, 0.0f, 1.0f);
const DColor dcol_green      = DColor(0.0f, 1.0f, 0.0f, 1.0f);
const DColor dcol_blue       = DColor(0.0f, 0.0f, 1.0f, 1.0f);
const DColor dcol_white      = DColor(1.0f, 1.0f, 1.0f, 1.0f);
const DColor dcol_black      = DColor(0.0f, 0.0f, 0.0f, 1.0f);
const DColor dcol_yellow     = DColor(1.0f, 1.0f, 0.0f, 1.0f);
const DColor dcol_cyan       = DColor(0.0f, 1.0f, 1.0f, 1.0f);
const DColor dcol_magenta    = DColor(1.0f, 0.0f, 1.0f, 1.0f);
const DColor dcol_gray       = DColor(0.5f, 0.5f, 0.5f, 1.0f);
const DColor dcol_clear      = DColor(0.0f, 0.0f, 0.0f, 0.0f);
