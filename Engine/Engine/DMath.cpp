#include "DMath.h"
#include <exception>
#include <Windows.h>

using namespace std;

DRect::DRect()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

DRect::DRect(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

bool DRect::Contains(float x, float y) const
{
	return x >= this->x && x <= this->x + this->width && y >= this->y && y <= this->y + this->height;
}

bool DRect::Contains(const DRect & rect) const
{
	return rect.x >= this->x && rect.x + rect.width <= this->x + this->width && rect.y >= this->y && rect.y + rect.height <= this->y + this->height;
}

bool DRect::Overlaps(const DRect & rect) const
{
	return rect.GetMaxX() > this->GetMinX() && rect.GetMinX() < this->GetMaxX() && rect.GetMaxY() > this->GetMinY() && rect.GetMinY() < this->GetMaxY();
}

void DRect::GetCenter(float & x, float & y) const
{
	x = this->x + this->width*0.5f;
	y = this->y + this->height*0.5f;
}

float DRect::GetMinX() const
{
	return x;
}

float DRect::GetMinY() const
{
	return y;
}

float DRect::GetMaxX() const
{
	return x + width;
}

float DRect::GetMaxY() const
{
	return y + height;
}

DVector2::DVector2()
{
	x = 0.0f;
	y = 0.0f;
}

DVector2::DVector2(const DVector2 & vector)
{
	x = vector.x;
	y = vector.y;
}

DVector2::DVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

float DVector2::Magnitude() const
{
	return sqrtf(x*x + y*y);
}

float DVector2::SqrMagnitude() const
{
	return x*x + y*y;
}

void DVector2::Normalize()
{
	float m = Magnitude();
	if (m > 0.0f)
	{
		x /= m;
		y /= m;
	}
	else {
		x = 0.0f;
		y = 0.0f;
	}
}

void DVector2::GetNormalized(DVector2& out) const
{
	out.x = x;
	out.y = y;
	out.Normalize();
}

void DVector2::Scale(const DVector2 & scale)
{
	x *= scale.x;
	y *= scale.y;
}

float& DVector2::operator[](int index)
{
	if (index < 0 || index >= 2)
	{
		throw std::exception("无效的索引");
	}
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	}
}

bool DVector2::operator==(const DVector2 & vector) const
{
	return IS_FLOAT_EQUAL(x, vector.x) && IS_FLOAT_EQUAL(y, vector.y);
}

bool DVector2::operator!=(const DVector2 & vector) const
{
	return !IS_FLOAT_EQUAL(x, vector.x) || !IS_FLOAT_EQUAL(y, vector.y);
}

DVector2 DVector2::operator+(const DVector2 & vector) const
{
	return DVector2(x + vector.x, y + vector.y);
}

DVector2 DVector2::operator-(const DVector2 & vector) const
{
	return DVector2(x - vector.x, y - vector.y);
}

DVector2 DVector2::operator*(float value) const
{
	return DVector2(x * value, y * value);
}

DVector2 DVector2::operator/(float value) const
{
	return DVector2(x / value, y / value);
}

DVector2 & DVector2::operator+=(const DVector2 & vector)
{
	x += vector.x;
	y += vector.y;
	return *this;
}

DVector2 & DVector2::operator-=(const DVector2 & vector)
{
	x -= vector.x;
	y -= vector.y;
	return *this;
}

DVector2 & DVector2::operator*=(float value)
{
	x *= value;
	y *= value;
	return *this;
}

DVector2 & DVector2::operator/=(float value)
{
	x /= value;
	y /= value;
	return *this;
}

void DVector2::Lerp(const DVector2 & a, const DVector2 & b, float t, DVector2& out)
{
	if (t < 0.0f)
		t = 0.0f;
	else if (t>1.0f)
		t = 1.0f;
	out = DVector2(a.x + (b.x - a.x)*t, a.y + (b.y - a.y)*t);
}

void DVector2::Reflect(const DVector2 & direction, const DVector2 & normal, DVector2& out)
{
	out = 2.0f * DVector2::Dot(normal, direction) * normal - direction;
}

float DVector2::Magnitude(const DVector2 & vector)
{
	return sqrtf(vector.x*vector.x + vector.y*vector.y);
}

float DVector2::SqrMagnitude(const DVector2 & vector)
{
	return vector.x*vector.x + vector.y*vector.y;
}

float DVector2::Angle(const DVector2 & fromvec, const DVector2 & tovec)
{
	DVector2 fn, tn;
	fromvec.GetNormalized(fn);
	tovec.GetNormalized(tn);
	float v = DVector2::Dot(fn, tn);
	if (v < -1.0f)
		v = -1.0f;
	else if (v > 1.0f)
		v = 1.0f;
	return acosf(v)*57.29578f;
}

float DVector2::Dot(const DVector2 & lhs, const DVector2 & rhs)
{
	return lhs.x * rhs.x + lhs.y + rhs.y;
}

float DVector2::Distance(const DVector2 & lhs, const DVector2 & rhs)
{
	return (lhs - rhs).Magnitude();
}

void DVector2::Max(const DVector2 & lhs, const DVector2 & rhs, DVector2& out)
{
	out = DVector2(max(lhs.x, rhs.x), max(lhs.y, rhs.y));
}

void DVector2::Min(const DVector2 & lhs, const DVector2 & rhs, DVector2& out)
{
	out = DVector2(min(lhs.x, rhs.x), min(lhs.y, rhs.y));
}

void DVector2::Normalize(const DVector2 & vector, DVector2& out)
{
	vector.GetNormalized(out);
}

void DVector2::Scale(const DVector2 & a, const DVector2 & b, DVector2& out)
{
	out = DVector2(a.x * b.x, a.y * b.y);
}

DVector2 operator*(float v, const DVector2 & vector)
{
	return DVector2(v * vector.x, v * vector.y);
}

DVector3 operator*(float v, const DVector3 & vector)
{
	return DVector3(v * vector.x, v * vector.y, v * vector.z);
}

DVector4 operator*(float v, const DVector4 & vector)
{
	return DVector4(v * vector.x, v * vector.y, v * vector.z, v * vector.w);
}

DVector4 operator*(const DVector4 & vector, const DMatrix4x4 & matrix)
{
	return DVector4(vector.x *matrix.m00 + vector.y *matrix.m10 + vector.z * matrix.m20 + vector.w*matrix.m30,
		vector.x *matrix.m01 + vector.y * matrix.m11 + vector.z *matrix.m21 + vector.w*matrix.m31,
		vector.x *matrix.m02 + vector.y *matrix.m12 + vector.z *matrix.m22 + vector.w*matrix.m32,
		vector.x *matrix.m03 + vector.y * matrix.m13 + vector.z *matrix.m23 + vector.w*matrix.m33);
}

DVector3::DVector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

DVector3::DVector3(const DVector3 & vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

DVector3::DVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float DVector3::Magnitude() const
{
	return sqrtf(x*x + y*y + z*z);
}

float DVector3::SqrMagnitude() const
{
	return x*x + y*y + z*z;
}

void DVector3::Normalize()
{
	float m = Magnitude();
	if (m > 0.0f)
	{
		x /= m;
		y /= m;
		z / m;
	}
	else {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
}

void DVector3::GetNormalized(DVector3& out) const
{
	out.x = x;
	out.y = y;
	out.z = z;
	out.Normalize();
}

void DVector3::Scale(const DVector3 & scale)
{
	x *= scale.x;
	y *= scale.y;
	z *= scale.z;
}

float& DVector3::operator[](int index)
{
	if (index < 0 || index >= 3)
	{
		throw std::exception("无效的索引");
	}
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}
}

bool DVector3::operator==(const DVector3 & vector) const
{
	return IS_FLOAT_EQUAL(x, vector.x) && IS_FLOAT_EQUAL(y, vector.y) && IS_FLOAT_EQUAL(z, vector.z);
}

bool DVector3::operator!=(const DVector3 & vector) const
{
	return !IS_FLOAT_EQUAL(x, vector.x) || !IS_FLOAT_EQUAL(y, vector.y) || !IS_FLOAT_EQUAL(z, vector.z);
}

DVector3 DVector3::operator+(const DVector3 & vector) const
{
	return DVector3(x + vector.x, y + vector.y, z + vector.z);
}

DVector3 DVector3::operator-(const DVector3 & vector) const
{
	return DVector3(x - vector.x, y - vector.y, z - vector.z);
}

DVector3 DVector3::operator*(float value) const
{
	return DVector3(x * value, y * value, z * value);
}

DVector3 DVector3::operator/(float value) const
{
	return DVector3(x / value, y / value, z / value);
}

DVector3 & DVector3::operator+=(const DVector3 & vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
	return *this;
}

DVector3 & DVector3::operator-=(const DVector3 & vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	return *this;
}

DVector3 & DVector3::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

DVector3 & DVector3::operator/=(float value)
{
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

void DVector3::Lerp(const DVector3 & a, const DVector3 & b, float t, DVector3& out)
{
	if (t < 0.0f)
		t = 0.0f;
	else if (t>1.0f)
		t = 1.0f;
	out = DVector3(a.x + (b.x - a.x)*t, a.y + (b.y - a.y)*t, a.z + (b.z - a.z)*t);
}

void DVector3::Reflect(const DVector3 & direction, const DVector3 & normal, DVector3& out)
{
	out = 2.0f * DVector3::Dot(normal, direction)*normal - direction;
}

float DVector3::Angle(const DVector3 & fromvec, const DVector3 & tovec)
{
	DVector3 fn, tn;
	fromvec.GetNormalized(fn);
	tovec.GetNormalized(tn);
	float v = DVector3::Dot(fn, tn);
	if (v < -1.0f)
		v = -1.0f;
	if (v > 1.0f)
		v = 1.0f;
	return acosf(v) * 57.29578f;
}

float DVector3::Dot(const DVector3 & lhs, const DVector3 & rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

float DVector3::Distance(const DVector3 & lhs, const DVector3 & rhs)
{
	DVector3 v = lhs - rhs;
	return v.Magnitude();
}

void DVector3::Max(const DVector3 & lhs, const DVector3 & rhs, DVector3& out)
{
	out = DVector3(max(lhs.x, rhs.x), max(lhs.y, rhs.y), max(lhs.z, rhs.z));
}

void DVector3::Min(const DVector3 & lhs, const DVector3 & rhs, DVector3& out)
{
	out = DVector3(min(lhs.x, rhs.x), min(lhs.y, rhs.y), min(lhs.z, rhs.z));
}

float DVector3::Magnitude(const DVector3 & vector)
{
	return sqrtf(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);;
}

float DVector3::SqrMagnitude(const DVector3 & vector)
{
	return vector.x*vector.x + vector.y*vector.y + vector.z*vector.z;
}

void DVector3::Normalize(const DVector3 & vector, DVector3& out)
{
	vector.GetNormalized(out);
}

void DVector3::Scale(const DVector3 & a, const DVector3 & b, DVector3& out)
{
	out = DVector3(a.x*b.x, a.y*b.y, a.z*b.z);
}

void DVector3::Cross(const DVector3 & lhs, const DVector3 & rhs, DVector3& out)
{
	out = DVector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

void DVector3::Project(const DVector3 & vector, const DVector3 & normal, DVector3& out)
{
	float n = DVector3::Dot(normal, normal);
	out = DVector3();
	if (n >= EPSILNON)
		out = normal* DVector3::Dot(vector, normal) / n;
}

void DVector3::ProjectOnPlane(const DVector3 & vector, const DVector3 & planeNormal, DVector3& out)
{
	DVector3 pj;
	DVector3::Project(vector, planeNormal, pj);
	out = vector - pj;
}

DVector4::DVector4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

DVector4::DVector4(const DVector4 & vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = vector.w;
}

DVector4::DVector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float DVector4::Magnitude() const
{
	return sqrtf(x*x + y*y + z*z + w*w);
}

float DVector4::SqrMagnitude() const
{
	return x*x + y*y + z*z + w*w;
}

void DVector4::Normalize()
{
	float m = Magnitude();
	if (m > 0.0f)
	{
		x /= m;
		y /= m;
		z /= m;
		w /= m;
	}
	else {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}
}

void DVector4::GetNormalized(DVector4& out) const
{
	out.x = x;
	out.y = y;
	out.z = z;
	out.w = w;
	out.Normalize();
}

void DVector4::Scale(const DVector4 & scale)
{
	x *= scale.x;
	y *= scale.y;
	z *= scale.z;
	w *= scale.w;
}

float& DVector4::operator[](int index)
{
	if (index < 0 || index >= 4)
	{
		throw std::exception("无效的索引");
	}
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
}

bool DVector4::operator==(const DVector4 & vector) const
{
	return IS_FLOAT_EQUAL(x, vector.x) && IS_FLOAT_EQUAL(y, vector.y) && IS_FLOAT_EQUAL(z, vector.z) && IS_FLOAT_EQUAL(w, vector.w);
}

bool DVector4::operator!=(const DVector4 & vector) const
{
	return !IS_FLOAT_EQUAL(x, vector.x) || !IS_FLOAT_EQUAL(y, vector.y) || !IS_FLOAT_EQUAL(z, vector.z) || !IS_FLOAT_EQUAL(w, vector.w);
}

DVector4 DVector4::operator+(const DVector4 & vector) const
{
	return DVector4(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
}

DVector4 DVector4::operator-(const DVector4 & vector) const
{
	return DVector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
}

DVector4 DVector4::operator*(float value) const
{
	return DVector4(x * value, y * value, z * value, w * value);
}

DVector4 DVector4::operator/(float value) const
{
	return DVector4(x / value, y / value, z / value, w / value);
}

DVector4 & DVector4::operator+=(const DVector4 & vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
	w += vector.w;
	return *this;
}

DVector4 & DVector4::operator-=(const DVector4 & vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	w -= vector.w;
	return *this;
}

DVector4 & DVector4::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
	return *this;
}

DVector4 & DVector4::operator/=(float value)
{
	x /= value;
	y /= value;
	z /= value;
	w /= value;
	return *this;
}

void DVector4::Lerp(const DVector4 & a, const DVector4 & b, float t, DVector4& out)
{
	if (t < 0.0f)
		t = 0.0f;
	else if (t>1.0f)
		t = 1.0f;
	out = DVector4(a.x + (b.x - a.x)*t, a.y + (b.y - a.y)*t, a.z + (b.z - a.z)*t, a.w + (b.w - a.w)*t);
}

float DVector4::Dot(const DVector4 & lhs, const DVector4 & rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
}

float DVector4::Distance(const DVector4 & lhs, const DVector4 & rhs)
{
	DVector4 v = lhs - rhs;
	return v.Magnitude();
}

void DVector4::Max(const DVector4 & lhs, const DVector4 & rhs, DVector4& out)
{
	out = DVector4(max(lhs.x, rhs.x), max(lhs.y, rhs.y), max(lhs.z, rhs.z), max(lhs.w, rhs.w));
}

void DVector4::Min(const DVector4 & lhs, const DVector4 & rhs, DVector4& out)
{
	out = DVector4(min(lhs.x, rhs.x), min(lhs.y, rhs.y), min(lhs.z, rhs.z), min(lhs.w, rhs.w));
}

float DVector4::Magnitude(const DVector4 & vector)
{
	return sqrtf(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z + vector.w*vector.w);
}

float DVector4::SqrMagnitude(const DVector4 & vector)
{
	return vector.x*vector.x + vector.y*vector.y + vector.z*vector.z + vector.w*vector.w;
}

void DVector4::Normalize(const DVector4 & vector, DVector4& out)
{
	vector.GetNormalized(out);
}

void DVector4::Scale(const DVector4 & a, const DVector4 & b, DVector4& out)
{
	out = DVector4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}

void DVector4::Project(const DVector4 & a, const DVector4 & b, DVector4& out)
{
	out = b * DVector4::Dot(a, b) / DVector4::Dot(b, b);
}

DQuaterion::DQuaterion()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

DQuaterion::DQuaterion(const DQuaterion & quaterion)
{
	x = quaterion.x;
	y = quaterion.y;
	z = quaterion.z;
	w = quaterion.w;
}

DQuaterion::DQuaterion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void DQuaterion::EulerAngle(float & eulerX, float & eulerY, float & eulerZ)
{
	long sp = -2.0f*(y*z - w*x);

	if (abs(sp) > 0.9999f)
	{
		eulerX = 1.570796f * sp;
		eulerY = atan2f(-x*z + w*y, 0.5f - y*y - z*z);
		eulerZ = 0.0f;
	}
	else
	{
		eulerX = asinf(sp);
		eulerY = atan2f(x*z + w*y, 0.5f - x*x - y*y);
		eulerZ = atan2f(x*y + w*z, 0.5f - x*x - z*z);
	}
	eulerX *= RAD_TO_DEG;
	eulerY *= RAD_TO_DEG;
	eulerZ *= RAD_TO_DEG;
}

void DQuaterion::EulerAngle(DVector3 & euler)
{
	float sp = -2.0f*(y*z - w*x);

	if (fabsf(sp) > 0.9999f)
	{
		euler.x = 1.570796f * sp;
		euler.y = atan2f(-x*z + w*y, 0.5f - y*y - z*z);
		euler.z = 0.0f;
	}
	else
	{
		euler.x = asinf(sp);
		euler.y = atan2f(x*z + w*y, 0.5f - x*x - y*y);
		euler.z = atan2f(x*y + w*z, 0.5f - x*x - z*z);
	}
	euler.x *= RAD_TO_DEG;
	euler.y *= RAD_TO_DEG;
	euler.z *= RAD_TO_DEG;
}

void DQuaterion::Euler(DQuaterion * rotation, float eulerX, float eulerY, float eulerZ)
{
	float cosx = cosf(eulerX*DEG_TO_RAD*0.5f);
	float cosy = cosf(eulerY*DEG_TO_RAD*0.5f);
	float cosz = cosf(eulerZ*DEG_TO_RAD*0.5f);

	float sinx = sinf(eulerX*DEG_TO_RAD*0.5f);
	float siny = sinf(eulerY*DEG_TO_RAD*0.5f);
	float sinz = sinf(eulerZ*DEG_TO_RAD*0.5f);

	rotation->x = cosy*sinx*cosz + siny*cosx*sinz;
	rotation->y = siny*cosx*cosz - cosy*sinx*sinz;
	rotation->z = cosy*cosx*sinz - siny*sinx*cosz;
	rotation->w = cosy*cosx*cosz + siny*sinx*sinz;
}

void DQuaterion::Euler(DQuaterion * rotation, const DVector3 & euler)
{
	float cosx = cosf(euler.x*DEG_TO_RAD*0.5f);
	float cosy = cosf(euler.y*DEG_TO_RAD*0.5f);
	float cosz = cosf(euler.z*DEG_TO_RAD*0.5f);

	float sinx = sinf(euler.x*DEG_TO_RAD*0.5f);
	float siny = sinf(euler.y*DEG_TO_RAD*0.5f);
	float sinz = sinf(euler.z*DEG_TO_RAD*0.5f);

	rotation->x = cosy*sinx*cosz + siny*cosx*sinz;
	rotation->y = siny*cosx*cosz - cosy*sinx*sinz;
	rotation->z = cosy*cosx*sinz - siny*sinx*cosz;
	rotation->w = cosy*cosx*cosz + siny*sinx*sinz;
}

DMatrix4x4::DMatrix4x4()
{
	m00 = 0.0f; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
	m10 = 0.0f; m11 = 0.0f; m12 = 0.0f; m13 = 0.0f;
	m20 = 0.0f; m21 = 0.0f; m22 = 0.0f; m23 = 0.0f;
	m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 0.0f;
}

DMatrix4x4::DMatrix4x4(const DMatrix4x4 & matrix)
{
	m00 = matrix.m00; m01 = matrix.m01; m02 = matrix.m02; m03 = matrix.m03;
	m10 = matrix.m10; m11 = matrix.m11; m12 = matrix.m12; m13 = matrix.m13;
	m20 = matrix.m20; m21 = matrix.m21; m22 = matrix.m22; m23 = matrix.m23;
	m30 = matrix.m30; m31 = matrix.m31; m32 = matrix.m32; m33 = matrix.m33;
}

DMatrix4x4::DMatrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
	this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
	this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
	this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
}

bool DMatrix4x4::operator==(const DMatrix4x4 & matrix) const
{
	if (!IS_FLOAT_EQUAL(m00, matrix.m00))
		return false;
	if (!IS_FLOAT_EQUAL(m01, matrix.m01))
		return false;
	if (!IS_FLOAT_EQUAL(m02, matrix.m02))
		return false;
	if (!IS_FLOAT_EQUAL(m03, matrix.m03))
		return false;
	if (!IS_FLOAT_EQUAL(m10, matrix.m10))
		return false;
	if (!IS_FLOAT_EQUAL(m11, matrix.m11))
		return false;
	if (!IS_FLOAT_EQUAL(m12, matrix.m12))
		return false;
	if (!IS_FLOAT_EQUAL(m13, matrix.m13))
		return false;
	if (!IS_FLOAT_EQUAL(m20, matrix.m20))
		return false;
	if (!IS_FLOAT_EQUAL(m21, matrix.m21))
		return false;
	if (!IS_FLOAT_EQUAL(m22, matrix.m22))
		return false;
	if (!IS_FLOAT_EQUAL(m23, matrix.m23))
		return false;
	if (!IS_FLOAT_EQUAL(m30, matrix.m30))
		return false;
	if (!IS_FLOAT_EQUAL(m31, matrix.m31))
		return false;
	if (!IS_FLOAT_EQUAL(m32, matrix.m32))
		return false;
	if (!IS_FLOAT_EQUAL(m33, matrix.m33))
		return false;
	return true;
}

bool DMatrix4x4::operator!=(const DMatrix4x4 & matrix) const
{
	if (!IS_FLOAT_EQUAL(m00, matrix.m00))
		return true;
	if (!IS_FLOAT_EQUAL(m01, matrix.m01))
		return true;
	if (!IS_FLOAT_EQUAL(m02, matrix.m02))
		return true;
	if (!IS_FLOAT_EQUAL(m03, matrix.m03))
		return true;
	if (!IS_FLOAT_EQUAL(m10, matrix.m10))
		return true;
	if (!IS_FLOAT_EQUAL(m11, matrix.m11))
		return true;
	if (!IS_FLOAT_EQUAL(m12, matrix.m12))
		return true;
	if (!IS_FLOAT_EQUAL(m13, matrix.m13))
		return true;
	if (!IS_FLOAT_EQUAL(m20, matrix.m20))
		return true;
	if (!IS_FLOAT_EQUAL(m21, matrix.m21))
		return true;
	if (!IS_FLOAT_EQUAL(m22, matrix.m22))
		return true;
	if (!IS_FLOAT_EQUAL(m23, matrix.m23))
		return true;
	if (!IS_FLOAT_EQUAL(m30, matrix.m30))
		return true;
	if (!IS_FLOAT_EQUAL(m31, matrix.m31))
		return true;
	if (!IS_FLOAT_EQUAL(m32, matrix.m32))
		return true;
	if (!IS_FLOAT_EQUAL(m33, matrix.m33))
		return true;
	return false;
}

DMatrix4x4 DMatrix4x4::operator*(const DMatrix4x4 & matrix) const
{
	float rm00 = m00*matrix.m00 + m01*matrix.m10 + m02*matrix.m20 + m03*matrix.m30;
	float rm01 = m00*matrix.m01 + m01*matrix.m11 + m02*matrix.m21 + m03*matrix.m31;
	float rm02 = m00*matrix.m02 + m01*matrix.m12 + m02*matrix.m22 + m03*matrix.m32;
	float rm03 = m00*matrix.m03 + m01*matrix.m13 + m02*matrix.m23 + m03*matrix.m33;

	float rm10 = m10*matrix.m00 + m11*matrix.m10 + m12*matrix.m20 + m13*matrix.m30;
	float rm11 = m10*matrix.m01 + m11*matrix.m11 + m12*matrix.m21 + m13*matrix.m31;
	float rm12 = m10*matrix.m02 + m11*matrix.m12 + m12*matrix.m22 + m13*matrix.m32;
	float rm13 = m10*matrix.m03 + m11*matrix.m13 + m12*matrix.m23 + m13*matrix.m33;

	float rm20 = m20*matrix.m00 + m21*matrix.m10 + m22*matrix.m20 + m23*matrix.m30;
	float rm21 = m20*matrix.m01 + m21*matrix.m11 + m22*matrix.m21 + m23*matrix.m31;
	float rm22 = m20*matrix.m02 + m21*matrix.m12 + m22*matrix.m22 + m23*matrix.m32;
	float rm23 = m20*matrix.m03 + m21*matrix.m13 + m22*matrix.m23 + m23*matrix.m33;

	float rm30 = m30*matrix.m00 + m31*matrix.m10 + m32*matrix.m20 + m33*matrix.m30;
	float rm31 = m30*matrix.m01 + m31*matrix.m11 + m32*matrix.m21 + m33*matrix.m31;
	float rm32 = m30*matrix.m02 + m31*matrix.m12 + m32*matrix.m22 + m33*matrix.m32;
	float rm33 = m30*matrix.m03 + m31*matrix.m13 + m32*matrix.m23 + m33*matrix.m33;

	return DMatrix4x4(rm00, rm01, rm02, rm03,
		rm10, rm11, rm12, rm13,
		rm20, rm21, rm22, rm23,
		rm30, rm31, rm32, rm33);
}

DMatrix4x4 & DMatrix4x4::operator*=(const DMatrix4x4 & matrix)
{
	float rm00 = m00*matrix.m00 + m01*matrix.m10 + m02*matrix.m20 + m03*matrix.m30;
	float rm01 = m00*matrix.m01 + m01*matrix.m11 + m02*matrix.m21 + m03*matrix.m31;
	float rm02 = m00*matrix.m02 + m01*matrix.m12 + m02*matrix.m22 + m03*matrix.m32;
	float rm03 = m00*matrix.m03 + m01*matrix.m13 + m02*matrix.m23 + m03*matrix.m33;

	float rm10 = m10*matrix.m00 + m11*matrix.m10 + m12*matrix.m20 + m13*matrix.m30;
	float rm11 = m10*matrix.m01 + m11*matrix.m11 + m12*matrix.m21 + m13*matrix.m31;
	float rm12 = m10*matrix.m02 + m11*matrix.m12 + m12*matrix.m22 + m13*matrix.m32;
	float rm13 = m10*matrix.m03 + m11*matrix.m13 + m12*matrix.m23 + m13*matrix.m33;

	float rm20 = m20*matrix.m00 + m21*matrix.m10 + m22*matrix.m20 + m23*matrix.m30;
	float rm21 = m20*matrix.m01 + m21*matrix.m11 + m22*matrix.m21 + m23*matrix.m31;
	float rm22 = m20*matrix.m02 + m21*matrix.m12 + m22*matrix.m22 + m23*matrix.m32;
	float rm23 = m20*matrix.m03 + m21*matrix.m13 + m22*matrix.m23 + m23*matrix.m33;

	float rm30 = m30*matrix.m00 + m31*matrix.m10 + m32*matrix.m20 + m33*matrix.m30;
	float rm31 = m30*matrix.m01 + m31*matrix.m11 + m32*matrix.m21 + m33*matrix.m31;
	float rm32 = m30*matrix.m02 + m31*matrix.m12 + m32*matrix.m22 + m33*matrix.m32;
	float rm33 = m30*matrix.m03 + m31*matrix.m13 + m32*matrix.m23 + m33*matrix.m33;

	m00 = rm00;
	m01 = rm01;
	m02 = rm02;
	m03 = rm03;
	m10 = rm10;
	m11 = rm11;
	m12 = rm12;
	m13 = rm13;
	m20 = rm20;
	m21 = rm21;
	m22 = rm22;
	m23 = rm23;
	m30 = rm30;
	m31 = rm31;
	m32 = rm32;
	m33 = rm33;
	return *this;
}

float & DMatrix4x4::operator[](int index)
{
	if (index < 0 || index >= 16)
	{
		throw std::exception("无效的索引");
	}
	switch (index)
	{
	case 0:
		return m00;
	case 1:
		return m01;
	case 2:
		return m02;
	case 3:
		return m03;
	case 4:
		return m10;
	case 5:
		return m11;
	case 6:
		return m12;
	case 7:
		return m13;
	case 8:
		return m20;
	case 9:
		return m21;
	case 10:
		return m22;
	case 11:
		return m23;
	case 12:
		return m30;
	case 13:
		return m31;
	case 14:
		return m32;
	case 15:
		return m33;
	}
}

void DMatrix4x4::TransformVector(const DVector3 & vector, DVector3 & out) const
{
	float x = vector.x * m00 + vector.y * m10 + vector.z * m20;
	float y = vector.x * m01 + vector.y * m11 + vector.z * m21;
	float z = vector.x * m02 + vector.y * m12 + vector.z * m22;

	out.x = x;
	out.y = y;
	out.z = z;
}

void DMatrix4x4::TransformPoint(const DVector3 & vector, DVector3 & out) const
{
	float x = vector.x * m00 + vector.y * m10 + vector.z * m20 + m30;
	float y = vector.x * m01 + vector.y * m11 + vector.z * m21 + m31;
	float z = vector.x * m02 + vector.y * m12 + vector.z * m22 + m32;
	float w = vector.x * m03 + vector.y * m13 + vector.z * m23 + m33;

	if (IS_FLOAT_EQUAL(w, 0.0f)) {
		out.x = NAN;
		out.y = NAN;
		out.z = NAN;
		return;
	}
	out.x = x / w;
	out.y = y / w;
	out.z = z / w;
}

void DMatrix4x4::GetTranspose(DMatrix4x4 & matrix) const
{
	matrix.m00 = m00;
	matrix.m01 = m10;
	matrix.m02 = m20;
	matrix.m03 = m30;

	matrix.m10 = m01;
	matrix.m11 = m11;
	matrix.m12 = m21;
	matrix.m13 = m31;

	matrix.m20 = m02;
	matrix.m21 = m12;
	matrix.m22 = m22;
	matrix.m23 = m32;

	matrix.m30 = m03;
	matrix.m31 = m13;
	matrix.m32 = m23;
	matrix.m33 = m33;
}

void DMatrix4x4::Transpose()
{
	float t01 = m01;
	m01 = m10;
	m10 = t01;

	float t02 = m02;
	m02 = m20;
	m20 = t02;

	float t03 = m03;
	m03 = m30;
	m30 = t03;

	float t12 = m12;
	m12 = m21;
	m21 = t12;

	float t13 = m13;
	m13 = m31;
	m31 = t13;

	float t23 = m23;
	m23 = m32;
	m32 = t23;
}

void DMatrix4x4::Inverse()
{
	DMatrix4x4 tmp;

	tmp[0] = m11 * m22 * m33 -
		m11 * m23 * m32 -
		m21 * m12 * m33 +
		m21 * m13 * m32 +
		m31 * m12 * m23 -
		m31 * m13 * m22;

	tmp[4] = -m10 * m22 * m33 +
		m10 * m23 * m32 +
		m20 * m12 * m33 -
		m20 * m13 * m32 -
		m30 * m12 * m23 +
		m30 * m13 * m22;

	tmp[8] = m10 * m21 * m33 -
		m10 * m23 * m31 -
		m20 * m11 * m33 +
		m20 * m13 * m31 +
		m30 * m11 * m23 -
		m30 * m13 * m21;

	tmp[12] = -m10 * m21 * m32 +
		m10 * m22 * m31 +
		m20 * m11 * m32 -
		m20 * m12 * m31 -
		m30 * m11 * m22 +
		m30 * m12 * m21;

	tmp[1] = -m01 * m22 * m33 +
		m01 * m23 * m32 +
		m21 * m02 * m33 -
		m21 * m03 * m32 -
		m31 * m02 * m23 +
		m31 * m03 * m22;

	tmp[5] = m00 * m22 * m33 -
		m00 * m23 * m32 -
		m20 * m02 * m33 +
		m20 * m03 * m32 +
		m30 * m02 * m23 -
		m30 * m03 * m22;

	tmp[9] = -m00 * m21 * m33 +
		m00 * m23 * m31 +
		m20 * m01 * m33 -
		m20 * m03 * m31 -
		m30 * m01 * m23 +
		m30 * m03 * m21;

	tmp[13] = m00 * m21 * m32 -
		m00 * m22 * m31 -
		m20 * m01 * m32 +
		m20 * m02 * m31 +
		m30 * m01 * m22 -
		m30 * m02 * m21;

	tmp[2] = m01 * m12 * m33 -
		m01 * m13 * m32 -
		m11 * m02 * m33 +
		m11 * m03 * m32 +
		m31 * m02 * m13 -
		m31 * m03 * m12;

	tmp[6] = -m00 * m12 * m33 +
		m00 * m13 * m32 +
		m10 * m02 * m33 -
		m10 * m03 * m32 -
		m30 * m02 * m13 +
		m30 * m03 * m12;

	tmp[10] = m00 * m11 * m33 -
		m00 * m13 * m31 -
		m10 * m01 * m33 +
		m10 * m03 * m31 +
		m30 * m01 * m13 -
		m30 * m03 * m11;

	tmp[14] = -m00 * m11 * m32 +
		m00 * m12 * m31 +
		m10 * m01 * m32 -
		m10 * m02 * m31 -
		m30 * m01 * m12 +
		m30 * m02 * m11;

	tmp[3] = -m01 * m12 * m23 +
		m01 * m13 * m22 +
		m11 * m02 * m23 -
		m11 * m03 * m22 -
		m21 * m02 * m13 +
		m21 * m03 * m12;

	tmp[7] = m00 * m12 * m23 -
		m00 * m13 * m22 -
		m10 * m02 * m23 +
		m10 * m03 * m22 +
		m20 * m02 * m13 -
		m20 * m03 * m12;

	tmp[11] = -m00 * m11 * m23 +
		m00 * m13 * m21 +
		m10 * m01 * m23 -
		m10 * m03 * m21 -
		m20 * m01 * m13 +
		m20 * m03 * m11;

	tmp[15] = m00 * m11 * m22 -
		m00 * m12 * m21 -
		m10 * m01 * m22 +
		m10 * m02 * m21 +
		m20 * m01 * m12 -
		m20 * m02 * m11;

	float det = m00 * tmp[0] + m01 * tmp[4] + m02 * tmp[8] + m03 * tmp[12];

	if (det == 0)
		throw std::exception("不可逆矩阵");

	det = 1.0f / det;

	for (int i = 0; i < 16; i++)
		this->operator[](i) = tmp[i] * det;
}

void DMatrix4x4::Perspective(DMatrix4x4 * matrix, float fov, float aspect, float nearplane, float farplane)
{
	float cotfov = 1.0f/tanf(fov*0.5f);
	float delta = farplane - nearplane;
	matrix->m00 = cotfov / aspect;
	matrix->m01 = 0.0f;
	matrix->m02 = 0.0f;
	matrix->m03 = 0.0f;

	matrix->m10 = 0.0f;
	matrix->m11 = cotfov;
	matrix->m12 = 0.0f;
	matrix->m13 = 0.0f;

	matrix->m20 = 0.0f;
	matrix->m21 = 0.0f;
	matrix->m22 = farplane / delta;
	matrix->m23 = 1.0f;

	matrix->m30 = 0.0f;
	matrix->m31 = 0.0f;
	matrix->m32 = -1.0f*nearplane*farplane/delta;
	matrix->m33 = 0.0f;
}

void DMatrix4x4::Ortho(DMatrix4x4 * matrix, float width, float height, float nearplane, float farplane)
{
	float fn = 1.0f / (farplane - nearplane);
	matrix->m00 = 2.0f / width;
	matrix->m01 = 0.0f;
	matrix->m02 = 0.0f;
	matrix->m03 = 0.0f;
	matrix->m10 = 0.0f;
	matrix->m11 = 2.0f / height;
	matrix->m12 = 0.0f;
	matrix->m13 = 0.0f;
	matrix->m20 = 0.0f;
	matrix->m21 = 0.0f;
	matrix->m22 = fn;
	matrix->m23 = 0.0f;
	matrix->m30 = 0.0f;
	matrix->m31 = 0.0f;
	matrix->m32 = -nearplane*fn;
	matrix->m33 = 1.0f;
}

void DMatrix4x4::Identity(DMatrix4x4 * matrix)
{
	matrix->m00 = 1;
	matrix->m01 = 0;
	matrix->m02 = 0;
	matrix->m03 = 0;
	matrix->m10 = 0;
	matrix->m11 = 1;
	matrix->m12 = 0;
	matrix->m13 = 0;
	matrix->m20 = 0;
	matrix->m21 = 0;
	matrix->m22 = 1;
	matrix->m23 = 0;
	matrix->m30 = 0;
	matrix->m31 = 0;
	matrix->m32 = 0;
	matrix->m33 = 1;
}

void DMatrix4x4::Scale(DMatrix4x4 * matrix, float x, float y, float z)
{
	matrix->m00 = x;
	matrix->m01 = 0;
	matrix->m02 = 0;
	matrix->m03 = 0;

	matrix->m10 = 0;
	matrix->m11 = y;
	matrix->m12 = 0;
	matrix->m13 = 0;

	matrix->m20 = 0;
	matrix->m21 = 0;
	matrix->m22 = z;
	matrix->m23 = 0;

	matrix->m30 = 0;
	matrix->m31 = 0;
	matrix->m32 = 0;
	matrix->m33 = 1;
}

void DMatrix4x4::Translate(DMatrix4x4 * matrix, float x, float y, float z)
{
	matrix->m00 = 1;
	matrix->m01 = 0;
	matrix->m02 = 0;
	matrix->m03 = 0;

	matrix->m10 = 0;
	matrix->m11 = 1;
	matrix->m12 = 0;
	matrix->m13 = 0;

	matrix->m20 = 0;
	matrix->m21 = 0;
	matrix->m22 = 1;
	matrix->m23 = 0;

	matrix->m30 = x;
	matrix->m31 = y;
	matrix->m32 = z;
	matrix->m33 = 1;
}

void DMatrix4x4::RotateX(DMatrix4x4 * matrix, float angle)
{
	float cosag = cosf(angle);
	float sinag = sinf(angle);
	matrix->m00 = 1.0f;
	matrix->m01 = 0.0f;
	matrix->m02 = 0.0f;
	matrix->m03 = 0.0f;
	matrix->m10 = 0.0f;
	matrix->m11 = cosag;
	matrix->m12 = sinag;
	matrix->m13 = 0.0f;
	matrix->m20 = 0.0f;
	matrix->m21 = -sinag;
	matrix->m22 = cosag;
	matrix->m23 = 0.0f;
	matrix->m30 = 0.0f;
	matrix->m31 = 0.0f;
	matrix->m32 = 0.0f;
	matrix->m33 = 1.0f;
}

void DMatrix4x4::RotateY(DMatrix4x4 * matrix, float angle)
{
	float cosag = cosf(angle);
	float sinag = sinf(angle);
	matrix->m00 = cosag;
	matrix->m01 = 0.0f;
	matrix->m02 = -sinag;
	matrix->m03 = 0.0f;
	matrix->m10 = 0.0f;
	matrix->m11 = 1.0f;
	matrix->m12 = 0.0f;
	matrix->m13 = 0.0f;
	matrix->m20 = sinag;
	matrix->m21 = 0.0f;
	matrix->m22 = cosag;
	matrix->m23 = 0.0f;
	matrix->m30 = 0.0f;
	matrix->m31 = 0.0f;
	matrix->m32 = 0.0f;
	matrix->m33 = 1.0f;
}

void DMatrix4x4::RotateZ(DMatrix4x4 * matrix, float angle)
{
	float cosag = cosf(angle);
	float sinag = sinf(angle);
	matrix->m00 = cosag;
	matrix->m01 = sinag;
	matrix->m02 = 0.0f;
	matrix->m03 = 0.0f;
	matrix->m10 = -sinag;
	matrix->m11 = cosag;
	matrix->m12 = 0.0f;
	matrix->m13 = 0.0f;
	matrix->m20 = 0.0f;
	matrix->m21 = 0.0f;
	matrix->m22 = 1.0f;
	matrix->m23 = 0.0f;
	matrix->m30 = 0.0f;
	matrix->m31 = 0.0f;
	matrix->m32 = 0.0f;
	matrix->m33 = 1.0f;
}

void DMatrix4x4::Rotate(DMatrix4x4 * matrix, const DQuaterion & rotation)
{
	float x2 = 2.0f*rotation.x*rotation.x;
	float y2 = 2.0f*rotation.y*rotation.y;
	float z2 = 2.0f*rotation.z*rotation.z;
	float xy = 2.0f*rotation.x*rotation.y;
	float xz = 2.0f*rotation.x*rotation.z;
	float xw = 2.0f*rotation.x*rotation.w;
	float yz = 2.0f*rotation.y*rotation.z;
	float yw = 2.0f*rotation.y*rotation.w;
	float zw = 2.0f*rotation.z*rotation.w;
	matrix->m00 = 1.0f - y2 - z2;
	matrix->m01 = xy + zw;
	matrix->m02 = xz - yw;
	matrix->m03 = 0.0f;
	matrix->m10 = xy - zw;
	matrix->m11 = 1.0f - x2 - z2;
	matrix->m12 = yz + xw;
	matrix->m13 = 0.0f;
	matrix->m20 = xz + yw;
	matrix->m21 = yz - xw;
	matrix->m22 = 1.0f - x2 - y2;
	matrix->m23 = 0.0f;
	matrix->m30 = 0.0f;
	matrix->m31 = 0.0f;
	matrix->m32 = 0.0f;
	matrix->m33 = 1.0f;
}

void DMatrix4x4::TRS(DMatrix4x4 * matrix, const DVector3 & position, const DQuaterion & rotation, const DVector3 & scale)
{
	float x2 = 2.0f*rotation.x*rotation.x;
	float y2 = 2.0f*rotation.y*rotation.y;
	float z2 = 2.0f*rotation.z*rotation.z;
	float xy = 2.0f*rotation.x*rotation.y;
	float xz = 2.0f*rotation.x*rotation.z;
	float xw = 2.0f*rotation.x*rotation.w;
	float yz = 2.0f*rotation.y*rotation.z;
	float yw = 2.0f*rotation.y*rotation.w;
	float zw = 2.0f*rotation.z*rotation.w;
	float ra = 1.0f - y2 - z2;
	float rb = xy + zw;
	float rc = xz - yw;
	float rd = xy - zw;
	float re = 1.0f - x2 - z2;
	float rf = yz + xw;
	float rg = xz + yw;
	float rh = yz - xw;
	float ri = 1.0f - x2 - y2;

	matrix->m00 = scale.x*ra;
	matrix->m01 = scale.x*rb;
	matrix->m02 = scale.x*rc;
	matrix->m03 = 0.0f;
	matrix->m10 = scale.y*rd;
	matrix->m11 = scale.y*re;
	matrix->m12 = scale.y*rf;
	matrix->m13 = 0.0f;
	matrix->m20 = scale.z*rg;
	matrix->m21 = scale.z*rh;
	matrix->m22 = scale.z*ri;
	matrix->m23 = 0.0f;
	matrix->m30 = position.x;
	matrix->m31 = position.y;
	matrix->m32 = position.z;
	matrix->m33 = 1.0f;
}

void DMatrix4x4::TRS(DMatrix4x4 * matrix, DVector3 * right, DVector3 * up, DVector3 * forward, const DVector3 & position, const DQuaterion & rotation, const DVector3 & scale)
{
	float x2 = 2.0f*rotation.x*rotation.x;
	float y2 = 2.0f*rotation.y*rotation.y;
	float z2 = 2.0f*rotation.z*rotation.z;
	float xy = 2.0f*rotation.x*rotation.y;
	float xz = 2.0f*rotation.x*rotation.z;
	float xw = 2.0f*rotation.x*rotation.w;
	float yz = 2.0f*rotation.y*rotation.z;
	float yw = 2.0f*rotation.y*rotation.w;
	float zw = 2.0f*rotation.z*rotation.w;
	float ra = 1.0f - y2 - z2;
	float rb = xy + zw;
	float rc = xz - yw;
	float rd = xy - zw;
	float re = 1.0f - x2 - z2;
	float rf = yz + xw;
	float rg = xz + yw;
	float rh = yz - xw;
	float ri = 1.0f - x2 - y2;

	right->x = ra;
	right->y = rb;
	right->z = rc;

	up->x = rd;
	up->y = re;
	up->z = rf;

	forward->x = rg;
	forward->y = rh;
	forward->z = ri;

	matrix->m00 = scale.x*ra;
	matrix->m01 = scale.x*rb;
	matrix->m02 = scale.x*rc;
	matrix->m03 = 0.0f;
	matrix->m10 = scale.y*rd;
	matrix->m11 = scale.y*re;
	matrix->m12 = scale.y*rf;
	matrix->m13 = 0.0f;
	matrix->m20 = scale.z*rg;
	matrix->m21 = scale.z*rh;
	matrix->m22 = scale.z*ri;
	matrix->m23 = 0.0f;
	matrix->m30 = position.x;
	matrix->m31 = position.y;
	matrix->m32 = position.z;
	matrix->m33 = 1.0f;
}

//void DMatrix4x4::TRS(DMatrix4x4 * matrix, DVector3 * forward, DVector3 * up, const DVector3 & position, const DQuaterion & rotation, const DVector3 & scale)
//{
//	float x2 = 2.0f*rotation.x*rotation.x;
//	float y2 = 2.0f*rotation.y*rotation.y;
//	float z2 = 2.0f*rotation.z*rotation.z;
//	float xy = 2.0f*rotation.x*rotation.y;
//	float xz = 2.0f*rotation.x*rotation.z;
//	float xw = 2.0f*rotation.x*rotation.w;
//	float yz = 2.0f*rotation.y*rotation.z;
//	float yw = 2.0f*rotation.y*rotation.w;
//	float zw = 2.0f*rotation.z*rotation.w;
//	float ra = 1.0f - y2 - z2;
//	float rb = xy + zw;
//	float rc = xz - yw;
//	float rd = xy - zw;
//	float re = 1.0f - x2 - z2;
//	float rf = yz + xw;
//	float rg = xz + yw;
//	float rh = yz - xw;
//	float ri = 1.0f - x2 - y2;
//
//	forward->x = rg;
//	forward->y = rh;
//	forward->z = ri;
//
//	up->x = rd;
//	up->y = re;
//	up->z = rf;
//
//	matrix->m00 = scale.x*ra;
//	matrix->m01 = scale.x*rb;
//	matrix->m02 = scale.x*rc;
//	matrix->m03 = 0.0f;
//	matrix->m10 = scale.y*rd;
//	matrix->m11 = scale.y*re;
//	matrix->m12 = scale.y*rf;
//	matrix->m13 = 0.0f;
//	matrix->m20 = scale.z*rg;
//	matrix->m21 = scale.z*rh;
//	matrix->m22 = scale.z*ri;
//	matrix->m23 = 0.0f;
//	matrix->m30 = position.x;
//	matrix->m31 = position.y;
//	matrix->m32 = position.z;
//	matrix->m33 = 1.0f;
//}

void DMatrix4x4::LookAt(DMatrix4x4 * matrix, const DVector3 & eye, const DVector3 & lookat, const DVector3 & up)
{
	DVector3 x, y, z;
	(lookat - eye).GetNormalized(z);
	DVector3::Cross(up, z, x);
	x.Normalize();
	DVector3::Cross(z, x, y);

	DMatrix4x4 o = DMatrix4x4(x.x, y.x, z.x, 0.0f,
		x.y, y.y, z.y, 0.0f,
		x.z, y.z, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	DMatrix4x4 t = DMatrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-eye.x, -eye.y, -eye.z, 1.0f);

	DVector3 be = eye*-1.0f;

	matrix->m00 = x.x;
	matrix->m01 = y.x;
	matrix->m02 = z.x;
	matrix->m03 = 0.0f;

	matrix->m10 = x.y;
	matrix->m11 = y.y;
	matrix->m12 = z.y;
	matrix->m13 = 0.0f;

	matrix->m20 = x.z;
	matrix->m21 = y.z;
	matrix->m22 = z.z;
	matrix->m23 = 0.0f;

	matrix->m30 = DVector3::Dot(x, be);
	matrix->m31 = DVector3::Dot(y, be);
	matrix->m32 = DVector3::Dot(z, be);
	matrix->m33 = 1.0f;
}

void DMatrix4x4::Transpose(DMatrix4x4 * out, const DMatrix4x4 & target)
{
	out->m00 = target.m00;
	out->m11 = target.m11;
	out->m22 = target.m22;
	out->m33 = target.m33;

	float t01, t02, t03, t10, t12, t13, t20, t21, t23, t30, t31, t32;
	t01 = target.m01;
	t02 = target.m02;
	t03 = target.m03;
	t10 = target.m10;
	t12 = target.m12;
	t13 = target.m13;
	t20 = target.m20;
	t21 = target.m21;
	t23 = target.m23;
	t30 = target.m30;
	t31 = target.m31;
	t32 = target.m32;

	out->m01 = t10;
	out->m02 = t20;
	out->m03 = t30;

	out->m10 = t01;
	out->m12 = t21;
	out->m13 = t31;

	out->m20 = t02;
	out->m21 = t12;
	out->m23 = t32;

	out->m30 = t03;
	out->m31 = t13;
	out->m32 = t23;
}

void DMatrix4x4::Inverse(DMatrix4x4 * out, const DMatrix4x4 & target)
{
	DMatrix4x4 tmp;

	tmp[0] = target.m11 * target.m22 * target.m33 -
		target.m11 * target.m23 * target.m32 -
		target.m21 * target.m12 * target.m33 +
		target.m21 * target.m13 * target.m32 +
		target.m31 * target.m12 * target.m23 -
		target.m31 * target.m13 * target.m22;

	tmp[4] = -target.m10 * target.m22 * target.m33 +
		target.m10 * target.m23 * target.m32 +
		target.m20 * target.m12 * target.m33 -
		target.m20 * target.m13 * target.m32 -
		target.m30 * target.m12 * target.m23 +
		target.m30 * target.m13 * target.m22;

	tmp[8] = target.m10 * target.m21 * target.m33 -
		target.m10 * target.m23 * target.m31 -
		target.m20 * target.m11 * target.m33 +
		target.m20 * target.m13 * target.m31 +
		target.m30 * target.m11 * target.m23 -
		target.m30 * target.m13 * target.m21;

	tmp[12] = -target.m10 * target.m21 * target.m32 +
		target.m10 * target.m22 * target.m31 +
		target.m20 * target.m11 * target.m32 -
		target.m20 * target.m12 * target.m31 -
		target.m30 * target.m11 * target.m22 +
		target.m30 * target.m12 * target.m21;

	tmp[1] = -target.m01 * target.m22 * target.m33 +
		target.m01 * target.m23 * target.m32 +
		target.m21 * target.m02 * target.m33 -
		target.m21 * target.m03 * target.m32 -
		target.m31 * target.m02 * target.m23 +
		target.m31 * target.m03 * target.m22;

	tmp[5] = target.m00 * target.m22 * target.m33 -
		target.m00 * target.m23 * target.m32 -
		target.m20 * target.m02 * target.m33 +
		target.m20 * target.m03 * target.m32 +
		target.m30 * target.m02 * target.m23 -
		target.m30 * target.m03 * target.m22;

	tmp[9] = -target.m00 * target.m21 * target.m33 +
		target.m00 * target.m23 * target.m31 +
		target.m20 * target.m01 * target.m33 -
		target.m20 * target.m03 * target.m31 -
		target.m30 * target.m01 * target.m23 +
		target.m30 * target.m03 * target.m21;

	tmp[13] = target.m00 * target.m21 * target.m32 -
		target.m00 * target.m22 * target.m31 -
		target.m20 * target.m01 * target.m32 +
		target.m20 * target.m02 * target.m31 +
		target.m30 * target.m01 * target.m22 -
		target.m30 * target.m02 * target.m21;

	tmp[2] = target.m01 * target.m12 * target.m33 -
		target.m01 * target.m13 * target.m32 -
		target.m11 * target.m02 * target.m33 +
		target.m11 * target.m03 * target.m32 +
		target.m31 * target.m02 * target.m13 -
		target.m31 * target.m03 * target.m12;

	tmp[6] = -target.m00 * target.m12 * target.m33 +
		target.m00 * target.m13 * target.m32 +
		target.m10 * target.m02 * target.m33 -
		target.m10 * target.m03 * target.m32 -
		target.m30 * target.m02 * target.m13 +
		target.m30 * target.m03 * target.m12;

	tmp[10] = target.m00 * target.m11 * target.m33 -
		target.m00 * target.m13 * target.m31 -
		target.m10 * target.m01 * target.m33 +
		target.m10 * target.m03 * target.m31 +
		target.m30 * target.m01 * target.m13 -
		target.m30 * target.m03 * target.m11;

	tmp[14] = -target.m00 * target.m11 * target.m32 +
		target.m00 * target.m12 * target.m31 +
		target.m10 * target.m01 * target.m32 -
		target.m10 * target.m02 * target.m31 -
		target.m30 * target.m01 * target.m12 +
		target.m30 * target.m02 * target.m11;

	tmp[3] = -target.m01 * target.m12 * target.m23 +
		target.m01 * target.m13 * target.m22 +
		target.m11 * target.m02 * target.m23 -
		target.m11 * target.m03 * target.m22 -
		target.m21 * target.m02 * target.m13 +
		target.m21 * target.m03 * target.m12;

	tmp[7] = target.m00 * target.m12 * target.m23 -
		target.m00 * target.m13 * target.m22 -
		target.m10 * target.m02 * target.m23 +
		target.m10 * target.m03 * target.m22 +
		target.m20 * target.m02 * target.m13 -
		target.m20 * target.m03 * target.m12;

	tmp[11] = -target.m00 * target.m11 * target.m23 +
		target.m00 * target.m13 * target.m21 +
		target.m10 * target.m01 * target.m23 -
		target.m10 * target.m03 * target.m21 -
		target.m20 * target.m01 * target.m13 +
		target.m20 * target.m03 * target.m11;

	tmp[15] = target.m00 * target.m11 * target.m22 -
		target.m00 * target.m12 * target.m21 -
		target.m10 * target.m01 * target.m22 +
		target.m10 * target.m02 * target.m21 +
		target.m20 * target.m01 * target.m12 -
		target.m20 * target.m02 * target.m11;

	float det = target.m00 * tmp[0] + target.m01 * tmp[4] + target.m02 * tmp[8] + target.m03 * tmp[12];

	if (det == 0)
		throw std::exception("不可逆矩阵");

	det = 1.0f / det;

	for (int i = 0; i < 16; i++)
		out->operator[](i) = tmp[i] * det;
}

DBounds::DBounds()
{
	center = DVector3(0.0f, 0.0f, 0.0f);
	size = DVector3(0.0f, 0.0f, 0.0f);
}

DBounds::DBounds(DVector3 center, DVector3 size)
{
	this->center = center;
	this->size = size;
}

void DBounds::GetMin(DVector3 * min) const
{
	min->x = min(center.x + size.x*0.5f, center.x - size.x*0.5f);
	min->y = min(center.y + size.y*0.5f, center.y - size.y*0.5f);
	min->z = min(center.z + size.z*0.5f, center.z - size.z*0.5f);
}

void DBounds::GetMax(DVector3 * max) const
{
	max->x = max(center.x + size.x*0.5f, center.x - size.x*0.5f);
	max->y = max(center.y + size.y*0.5f, center.y - size.y*0.5f);
	max->z = max(center.z + size.z*0.5f, center.z - size.z*0.5f);
}

DRay::DRay()
{
}

DRay::DRay(DVector3 origin, DVector3 direction)
{
	this->origin = origin;
	this->direction = direction;
}
