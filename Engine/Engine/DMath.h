﻿#pragma once
#include <math.h>

#define EPSILNON 0.000001f
#define D_PI 3.1415926535897f          //π
#define RAD_TO_DEG 57.29577951308f    //角度转弧度
#define DEG_TO_RAD 0.01745329252f     //弧度转角度

#define IS_FLOAT_EQUAL(a, b) (a>=b-EPSILNON&&a<=b+EPSILNON)   //浮点数比较

inline float clamp(float value, float min, float max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return value;
}

inline float clamp01(float value)
{
	return clamp(value, 0.0f, 1.0f);
}

inline float lerp(float a, float b, float t)
{
	return a + (b - a)*t;
}

typedef struct DRect
{
public:
	DRect();
	DRect(const DRect&);
	DRect(float, float, float, float);
	bool Contains(float x, float y) const;
	bool Contains(const DRect& rect) const;
	bool Overlaps(const DRect& rect) const;
	void GetCenter(float& x, float& y) const;
	float GetMinX() const;
	float GetMinY() const;
	float GetMaxX() const;
	float GetMaxY() const;

public:
	float x, y, width, height;
} DRect, *LPDRect;

typedef struct DVector2
{
public:
	DVector2();
	DVector2(const DVector2&);
	DVector2(float, float);

	float Magnitude() const;
	float SqrMagnitude() const;
	void Normalize();
	void GetNormalized(DVector2&) const;
	void Scale(const DVector2&);

	float& operator [](int);
	bool operator ==(const DVector2&) const;
	bool operator !=(const DVector2&) const;
	DVector2 operator +(const DVector2&) const;
	DVector2 operator-(const DVector2&) const;
	DVector2 operator*(float) const;
	DVector2 operator/(float) const;

	friend DVector2 operator * (float, const DVector2&);

	DVector2& operator += (const DVector2&);
	DVector2& operator -= (const DVector2&);
	DVector2& operator *= (float);
	DVector2& operator /= (float);

	static void Lerp(const DVector2&, const DVector2&, float, DVector2&);
	static void Reflect(const DVector2&, const DVector2&, DVector2&);
	static float Magnitude(const DVector2&);
	static float SqrMagnitude(const DVector2&);
	static float Angle(const DVector2&, const DVector2&);
	static float Dot(const DVector2&, const DVector2&);
	static float Distance(const DVector2&, const DVector2&);
	static void Max(const DVector2&, const DVector2&, DVector2&);
	static void Min(const DVector2&, const DVector2&, DVector2&);
	static void Normalize(const DVector2&, DVector2&);
	static void Scale(const DVector2&, const DVector2&, DVector2&);


public:
	float x, y;
} DVector2, *LPDVector2;

typedef struct DVector3
{
public:
	DVector3();
	DVector3(const DVector3&);
	DVector3(float, float, float);

	float Magnitude() const;
	float SqrMagnitude() const;
	void Normalize();
	void GetNormalized(DVector3&) const;
	void Scale(const DVector3&);
	float& operator [](int);
	bool operator ==(const DVector3&) const;
	bool operator !=(const DVector3&) const;
	DVector3 operator +(const DVector3&) const;
	DVector3 operator-(const DVector3&) const;
	DVector3 operator*(float) const;
	DVector3 operator/(float) const;

	friend DVector3 operator * (float, const DVector3&);

	DVector3& operator += (const DVector3&);
	DVector3& operator -= (const DVector3&);
	DVector3& operator *= (float);
	DVector3& operator /= (float);

	static void Lerp(const DVector3&, const DVector3&, float, DVector3&);
	static void Reflect(const DVector3&, const DVector3&, DVector3&);
	static float Angle(const DVector3&, const DVector3&);
	static float Dot(const DVector3&, const DVector3&);
	static float Distance(const DVector3&, const DVector3&);
	static void Max(const DVector3&, const DVector3&, DVector3&);
	static void Min(const DVector3&, const DVector3&, DVector3&);
	static float Magnitude(const DVector3&);
	static float SqrMagnitude(const DVector3&);
	static void Normalize(const DVector3&, DVector3&);
	static void Scale(const DVector3&, const DVector3&, DVector3&);
	static void Cross(const DVector3& lhs, const DVector3& rhs, DVector3& out);
	static void Project(const DVector3&, const DVector3&, DVector3&);
	static void ProjectOnPlane(const DVector3&, const DVector3&, DVector3&);

public:
	float x, y, z;
} DVector3, *LPDVector3;

typedef struct DVector4
{
public:
	DVector4();
	DVector4(const DVector4&);
	DVector4(float, float, float, float);

	float Magnitude() const;
	float SqrMagnitude() const;
	void Normalize();
	void GetNormalized(DVector4&) const;
	void Scale(const DVector4&);
	float& operator [](int);
	bool operator ==(const DVector4&) const;
	bool operator !=(const DVector4&) const;
	DVector4 operator +(const DVector4&) const;
	DVector4 operator-(const DVector4&) const;
	DVector4 operator*(float) const;
	DVector4 operator/(float) const;

	friend DVector4 operator * (float, const DVector4&);

	DVector4& operator += (const DVector4&);
	DVector4& operator -= (const DVector4&);
	DVector4& operator *= (float);
	DVector4& operator /= (float);

	static void Lerp(const DVector4&, const DVector4&, float, DVector4&);
	static float Dot(const DVector4&, const DVector4&);
	static float Distance(const DVector4&, const DVector4&);
	static void Max(const DVector4&, const DVector4&, DVector4&);
	static void Min(const DVector4&, const DVector4&, DVector4&);
	static float Magnitude(const DVector4&);
	static float SqrMagnitude(const DVector4&);
	static void Normalize(const DVector4&, DVector4&);
	static void Scale(const DVector4&, const DVector4&, DVector4&);
	static void Project(const DVector4&, const DVector4&, DVector4&);

public:
	float x, y, z, w;
} DVector4, *LPDVector4;

typedef struct DQuaternion
{
public:
	DQuaternion();
	DQuaternion(const DQuaternion&);
	DQuaternion(float, float, float, float);
	void EulerAngle(float & eulerX, float & eulerY, float & eulerZ);
	void EulerAngle(DVector3& euler);
	float Magnitude() const;

	float& operator [](int);
	bool operator ==(const DVector4&) const;
	bool operator !=(const DVector4&) const;
	DQuaternion operator * (const DQuaternion&) const;
	DQuaternion& operator *= (const DQuaternion&);

	static void Lerp(const DQuaternion&, const DQuaternion&, float, DQuaternion&);
	static void Slerp(const DQuaternion&, const DQuaternion&, float, DQuaternion&);
	static float Dot(const DQuaternion a, const DQuaternion b);
	static void Euler(DQuaternion* rotation, float, float, float);
	static void Euler(DQuaternion* rotation, const DVector3&);
	static float Magnitude(const DQuaternion&);
	static void Inverse(const DQuaternion&, DQuaternion&);

public:
	float x, y, z, w;
} DQuaternion, *LPDQuaternion;

typedef struct DMatrix4x4
{
public:
	DMatrix4x4();
	DMatrix4x4(const DMatrix4x4&);
	DMatrix4x4(float, float, float, float,
		float, float, float, float,
		float, float, float, float,
		float, float, float, float);

	bool operator == (const DMatrix4x4&) const;
	bool operator != (const DMatrix4x4&) const;
	DMatrix4x4 operator * (const DMatrix4x4&) const;
	DMatrix4x4& operator *= (const DMatrix4x4&);
	float& operator [](int index);
	friend DVector4 operator*(const DVector4&, const DMatrix4x4&);

	void TransformVector(const DVector3&, DVector3&) const;
	void TransformPoint(const DVector3&, DVector3&) const;
	void GetTranspose(DMatrix4x4&) const; 
	void Transpose();
	void Inverse();

	static void Perspective(DMatrix4x4*, float fov, float aspect, float near, float far);
	static void Ortho(DMatrix4x4*, float width, float height, float near, float far);
	static void Identity(DMatrix4x4*);
	static void Scale(DMatrix4x4*, float, float, float);
	static void Translate(DMatrix4x4*, float, float, float);
	static void RotateX(DMatrix4x4*, float);
	static void RotateY(DMatrix4x4*, float);
	static void RotateZ(DMatrix4x4*, float);
	static void Rotate(DMatrix4x4*, const DQuaternion&);
	static void TRS(DMatrix4x4*, const DVector3&, const DQuaternion&, const DVector3&);
	//static void TRS(DMatrix4x4*, DVector3* right, DVector3* up, DVector3* forward, const DVector3&, const DQuaterion&, const DVector3&);
	static void LookAt(DMatrix4x4*, const DVector3&, const DVector3&, const DVector3&);
	static void Transpose(DMatrix4x4* out, const DMatrix4x4& target);
	static void Inverse(DMatrix4x4* out, const DMatrix4x4& target);

public:
	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;
} DMatrix4x4, *LPDMatrix4x4;

typedef struct DRay
{
public:
	DRay();
	DRay(const DRay&);
	DRay(DVector3 origin, DVector3 direction);
	void GetPoint(float t, DVector3* out) const;

public:
	DVector3 origin;
	DVector3 direction;
} DRay;

typedef struct DBounds
{
public:
	DBounds();
	DBounds(const DBounds&);
	DBounds(float maxX, float maxY, float maxZ, float minX, float minY, float minZ);
	DBounds(DVector3 center, DVector3 size);
	void GetMin(DVector3* min) const;
	void GetMax(DVector3* max) const;
	void GetSize(DVector3* size) const;
	void GetClosestPoint(const DVector3&, DVector3* out) const;
	bool Contains(const DVector3&) const;
	bool Contains(const DBounds&) const;
	void Encapsulate(const DVector3&);
	void Encapsulate(const DBounds&);
	void Expand(float amound);
	bool IntersectRay(const DRay&, float*) const;
	bool IntersectRay(const DRay&) const;
	bool Intersects(const DBounds&) const;

public:
	DVector3 center;
	DVector3 halfSize;
} DBounds, *LPDBounds;

#define DVEC2_ZERO DVector2(0.0f, 0.0f)
#define DVEC2_ONE DVector2(1.0f, 1.0f)
#define DVEC2_UP DVector2(0.0f, 1.0f)
#define DVEC2_DOWN DVector2(0.0f, -1.0f)
#define DVEC2_LEFT DVector2(1.0f, 0.0f)
#define DVEC2_RIGHT DVector2(-1.0f, 0.0f)

#define DVEC3_ZERO DVector3(0.0f, 0.0f, 0.0f)
#define DVEC3_ONE DVector3(1.0f, 1.0f, 1.0f)
#define DVEC3_FORWARD DVector3(0.0f, 0.0f, 1.0f)
#define DVEC3_BACK DVector3(0.0f, 0.0f, -1.0f)
#define DVEC3_UP DVector3(0.0f, 1.0f, 0.0f)
#define DVEC3_DOWN DVector3(0.0f, -1.0f, 0.0f)
#define DVEC3_LEFT DVector3(-1.0f, 0.0f, 0.0f)
#define DVEC3_RIGHT DVector3(1.0f, 0.0f, 0.0f)

#define DVEC4_ZERO DVector4(0.0f, 0.0f, 0.0f, 0.0f)
#define DVEC4_ONE DVector4(1.0f, 1.0f, 1.0f, 1.0f)

#define DMATRIX_IDENTITY = DMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

#define DQUATERNION_IDENTITY = DQuaternion(0, 0, 0, 1)


const DVector2 dvec2_zero = DVector2(0.0f, 0.0f);
const DVector2 dvec2_one = DVector2(1.0f, 1.0f);
const DVector2 dvec2_up = DVector2(0.0f, 1.0f);
const DVector2 dvec2_down = DVector2(0.0f, -1.0f);
const DVector2 dvec2_left = DVector2(1.0f, 0.0f);
const DVector2 dvec2_right = DVector2(-1.0f, 0.0f);

const DVector3 dvec3_zero = DVector3(0.0f, 0.0f, 0.0f);
const DVector3 dvec3_one = DVector3(1.0f, 1.0f, 1.0f);
const DVector3 dvec3_forward = DVector3(0.0f, 0.0f, 1.0f);
const DVector3 dvec3_back = DVector3(0.0f, 0.0f, -1.0f);
const DVector3 dvec3_up = DVector3(0.0f, 1.0f, 0.0f);
const DVector3 dvec3_down = DVector3(0.0f, -1.0f, 0.0f);
const DVector3 dvec3_left = DVector3(-1.0f, 0.0f, 0.0f);
const DVector3 dvec3_right = DVector3(1.0f, 0.0f, 0.0f);

const DVector4 dvec4_zero = DVector4(0.0f, 0.0f, 0.0f, 0.0f);
const DVector4 dvec4_one = DVector4(1.0f, 1.0f, 1.0f, 1.0f);

const DMatrix4x4 dmat_identity = DMatrix4x4(1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);

const DQuaternion dquaternion_identity = DQuaternion(0, 0, 0, 1);