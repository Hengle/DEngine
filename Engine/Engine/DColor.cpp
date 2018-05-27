#include "DColor.h"
#include "DMath.h"
#include<exception> 

DColor::DColor() : DColor(1.0f,1.0f,1.0f,1.0f)
{
}

DColor::DColor(const DColor & color)
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

DColor::DColor(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

DColor::DColor(float red, float green, float blue) : DColor(red, green, blue, 1.0f)
{
}

float DColor::GetGrayScale()
{
	return 0.299f * r + 0.587f * g + 0.114f * b;
}

void DColor::ToHSV(float & h, float & s, float & v)
{
	DColor::RGBToHSV(*this, h, s, v);
}

DColor DColor::operator+(const DColor & color) const
{
	return DColor(color.r + r, color.g + g, color.b + b, color.a + a);
}

DColor DColor::operator-(const DColor & color) const
{
	return DColor(r - color.r, g - color.g, b - color.b, a - color.a);
}

DColor DColor::operator*(const DColor & color) const
{
	return DColor(r * color.r, g * color.g, b * color.b, a * color.a);
}

DColor DColor::operator*(const float  scale) const
{
	return DColor(r * scale, g * scale, b * scale, a * scale);
}

DColor DColor::operator/(const DColor & color) const
{
	return DColor(r / color.r, g / color.g, b / color.b, a / color.a);
}

DColor DColor::operator/(const float scale) const
{
	return DColor(r / scale, g / scale, b / scale, a / scale);
}

DColor & DColor::operator+=(const DColor & color)
{
	r = r + color.r;
	g = g + color.g;
	b = b + color.b;
	a = a + color.a;
	return *this;
}

DColor & DColor::operator-=(const DColor & color)
{
	r = r - color.r;
	g = g - color.g;
	b = b - color.b;
	a = a - color.a;
	return *this;
}

DColor & DColor::operator*=(const DColor & color)
{
	r = r * color.r;
	g = g * color.g;
	b = b * color.b;
	a = a * color.a;
	return *this;
}

DColor & DColor::operator*=(const float scale)
{
	r = r * scale;
	g = g * scale;
	b = b * scale;
	a = a * scale;
	return *this;
}

DColor & DColor::operator/=(const DColor & color)
{
	r = r / color.r;
	g = g / color.g;
	b = b / color.b;
	a = a / color.a;
	return *this;
}

DColor & DColor::operator/=(const float scale)
{
	r = r / scale;
	g = g / scale;
	b = b / scale;
	a = a / scale;
	return *this;
}

bool DColor::operator==(const DColor & color) const 
{
	return IS_FLOAT_EQUAL(r, color.r) && IS_FLOAT_EQUAL(g, color.g) && IS_FLOAT_EQUAL(b, color.b) && IS_FLOAT_EQUAL(a, color.a);
}

bool DColor::operator!=(const DColor & color) const
{
	return !(IS_FLOAT_EQUAL(r, color.r)) || !(IS_FLOAT_EQUAL(g, color.g)) || !(IS_FLOAT_EQUAL(b, color.b)) || !(IS_FLOAT_EQUAL(a, color.a));
}

float DColor::operator[](int index) const
{
	if (index < 0 || index >= 4)
	{
		throw std::exception("无效的索引");
	}
	switch (index)
	{
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	case 3:
		return a;
	default:
		return 0.0f;
	}
}

DColor DColor::Lerp(const DColor& a, const DColor& b, float t)
{
	return DColor(a.r + (b.r - a.r)*t, a.g + (b.g - a.g)*t, a.b + (b.b - a.b)*t, a.a + (b.a- a.a)*t);
}

DColor DColor::HSVToRGB(float h, float s, float v)
{
	DColor col = dcol_white;
	if (IS_FLOAT_EQUAL(s, 0.0f))
	{
		col.r = v;
		col.g = v;
		col.b = v;
	}
	else if (IS_FLOAT_EQUAL(v, 0.0f))
	{
		col.r = 0.0f;
		col.g = 0.0f;
		col.b = 0.0f;
	}
	else
	{
		col.r = 0.0f;
		col.g = 0.0f;
		col.b = 0.0f;
		float num = h * 6.0f;
		int num2 = (int)floor(num);
		float num3 = num - (float)num2;
		float num4 = v * (1.0f - s);
		float num5 = v * (1.0f - s * num3);
		float num6 = v * (1.0f - s * (1.0f - num3));
		switch (num2 + 1)
		{
		case 0:
			col.r = v;
			col.g = num4;
			col.b = num5;
			break;
		case 1:
			col.r = v;
			col.g = num6;
			col.b = num4;
			break;
		case 2:
			col.r = num5;
			col.g = v;
			col.b = num4;
			break;
		case 3:
			col.r = num4;
			col.g = v;
			col.b = num6;
			break;
		case 4:
			col.r = num4;
			col.g = num5;
			col.b = v;
			break;
		case 5:
			col.r = num6;
			col.g = num4;
			col.b = v;
			break;
		case 6:
			col.r = v;
			col.g = num4;
			col.b = num5;
			break;
		case 7:
			col.r = v;
			col.g = num6;
			col.b = num4;
			break;
		}
	}
	return col;
}

void DColor::RGBToHSV(const DColor& color, float & h, float & s, float & v)
{
	if (color.b > color.g && color.b > color.r)
	{
		DColor::RGBToHSV_Internal(4.0f, color.b, color.r, color.g, h, s, v);
	}
	else if (color.g > color.r)
	{
		DColor::RGBToHSV_Internal(2.0f, color.g, color.b, color.r, h, s, v);
	}
	else
	{
		DColor::RGBToHSV_Internal(0.0f, color.r, color.g, color.b, h, s, v);
	}
}

void DColor::RGBToHSV_Internal(float offset, float dominantcolor, float colorone, float colortwo, float & h, float & s, float & v)
{
	v = dominantcolor;
	if (!IS_FLOAT_EQUAL(v, 0.0f))
	{
		float num;
		if (colorone > colortwo)
		{
			num = colortwo;
		}
		else
		{
			num = colorone;
		}
		float num2 = v - num;
		if (!IS_FLOAT_EQUAL(num2, 0.0f))
		{
			s = num2 / v;
			h = offset + (colorone - colortwo) / num2;
		}
		else
		{
			s = 0.0f;
			h = offset + (colorone - colortwo);
		}
		h /= 6.0f;
		if (h < 0.0f)
		{
			h += 1.0f;
		}
	}
	else
	{
		s = 0.0f;
		h = 0.0f;
	}
}

DColor operator*(float scale, const DColor & color)
{
	return DColor(color.r * scale, color.g * scale, color.b * scale, color.a * scale);
}
