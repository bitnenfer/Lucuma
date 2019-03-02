#pragma once

#include "types.h"
#include <math.h>

namespace lu
{
	struct Vec2;
	struct Vec3;
	struct Vec4;
	struct Mat2;
	struct Mat3;
	struct Mat4;
}

// Global operators
lu::Mat2 operator*(const lu::Mat2& a, const lu::Mat2& b);
lu::Mat3 operator*(const lu::Mat3& a, const lu::Mat3& b);
lu::Mat4 operator*(const lu::Mat4& a, const lu::Mat4& b);

// Vec2
lu::Vec2 operator+(const lu::Vec2& a, const lu::Vec2& b);
lu::Vec2 operator-(const lu::Vec2& a, const lu::Vec2& b);
lu::Vec2 operator*(const lu::Vec2& a, const lu::Vec2& b);
lu::Vec2 operator/(const lu::Vec2& a, const lu::Vec2& b);
lu::Vec2 operator*(const lu::Mat2& a, const lu::Vec2& b);
lu::Vec2 operator*(const lu::Mat2& a, const lu::Vec2& b);
lu::Vec2 operator+(const lu::Vec2& a, const float32_t& b);
lu::Vec2 operator-(const lu::Vec2& a, const float32_t& b);
lu::Vec2 operator*(const lu::Vec2& a, const float32_t& b);
lu::Vec2 operator/(const lu::Vec2& a, const float32_t& b);
lu::Vec2 operator+(const float32_t& a, const lu::Vec2& b);
lu::Vec2 operator-(const float32_t& a, const lu::Vec2& b);
lu::Vec2 operator*(const float32_t& a, const lu::Vec2& b);
lu::Vec2 operator/(const float32_t& a, const lu::Vec2& b);
lu::Vec2 operator-(const lu::Vec2& a);

// Vec3
lu::Vec3 operator+(const lu::Vec3& a, const lu::Vec3& b);
lu::Vec3 operator-(const lu::Vec3& a, const lu::Vec3& b);
lu::Vec3 operator*(const lu::Vec3& a, const lu::Vec3& b);
lu::Vec3 operator/(const lu::Vec3& a, const lu::Vec3& b);
lu::Vec3 operator*(const lu::Mat3& a, const lu::Vec3& b);
lu::Vec3 operator+(const lu::Vec3& a, const float32_t& b);
lu::Vec3 operator-(const lu::Vec3& a, const float32_t& b);
lu::Vec3 operator*(const lu::Vec3& a, const float32_t& b);
lu::Vec3 operator/(const lu::Vec3& a, const float32_t& b);
lu::Vec3 operator+(const float32_t& a, const lu::Vec3& b);
lu::Vec3 operator-(const float32_t& a, const lu::Vec3& b);
lu::Vec3 operator*(const float32_t& a, const lu::Vec3& b);
lu::Vec3 operator/(const float32_t& a, const lu::Vec3& b);
lu::Vec3 operator-(const lu::Vec3& a);

// Vec4
lu::Vec4 operator+(const lu::Vec4& a, const lu::Vec4& b);
lu::Vec4 operator-(const lu::Vec4& a, const lu::Vec4& b);
lu::Vec4 operator*(const lu::Vec4& a, const lu::Vec4& b);
lu::Vec4 operator/(const lu::Vec4& a, const lu::Vec4& b);
lu::Vec4 operator*(const lu::Mat4& a, const lu::Vec4& b);
lu::Vec4 operator+(const lu::Vec4& a, const float32_t& b);
lu::Vec4 operator-(const lu::Vec4& a, const float32_t& b);
lu::Vec4 operator*(const lu::Vec4& a, const float32_t& b);
lu::Vec4 operator/(const lu::Vec4& a, const float32_t& b);
lu::Vec4 operator+(const float32_t& a, const lu::Vec4& b);
lu::Vec4 operator-(const float32_t& a, const lu::Vec4& b);
lu::Vec4 operator*(const float32_t& a, const lu::Vec4& b);
lu::Vec4 operator/(const float32_t& a, const lu::Vec4& b);
lu::Vec4 operator-(const lu::Vec4& a);

namespace lu
{
	const float32_t PI = 3.14159265359f;

	__forceinline float32_t Rad(float32_t d) { return PI * d / 180.0f; }
	__forceinline float32_t Deg(float32_t r) { return 180.0f * r / PI; }
	__forceinline float32_t Cos(float32_t t) { return cosf(t); }
	__forceinline float32_t Sin(float32_t t) { return sinf(t); }
	__forceinline float32_t Tan(float32_t t) { return tanf(t); }
	__forceinline float32_t Asin(float32_t t) { return asinf(t); }
	__forceinline float32_t Acos(float32_t t) { return acosf(t); }
	__forceinline float32_t Atan(float32_t t) { return atanf(t); }
	__forceinline float32_t Atan2(float32_t y, float32_t x) { return atan2f(y, x); }
	__forceinline float32_t Pow(float32_t x, float32_t y) { return powf(x, y); }
	__forceinline float32_t Exp(float32_t x) { return expf(x); }
	__forceinline float32_t Exp2(float32_t x) { return exp2f(x); }
	__forceinline float32_t Log(float32_t x) { return logf(x); }
	__forceinline float32_t Log2(float32_t x) { return log2f(x); }
	__forceinline float32_t Sqrt(float32_t x) { return sqrtf(x); }
	__forceinline float32_t InvSqrt(float32_t x) { return 1.0f / sqrtf(x); }
	__forceinline float32_t Abs(float32_t x) { return fabsf(x); }
	__forceinline float32_t Sign(float32_t x) { return (x < 0.0f ? -1.0f : 1.0f); }
	__forceinline float32_t Floor(float32_t x) { return floorf(x); }
	__forceinline float32_t Ceil(float32_t x) { return ceilf(x); }
	__forceinline float32_t Fract(float32_t x) { return x - floorf(x); }
	__forceinline float32_t Mod(float32_t x, float32_t y) { return fmodf(x, y); }
	__forceinline float32_t Min(float32_t x, float32_t y) { return (x < y ? x : y); }
	__forceinline float32_t Max(float32_t x, float32_t y) { return (x > y ? x : y); }
	__forceinline float32_t Clamp(float32_t n, float32_t x, float32_t y) { return (n < x ? x : n > y ? y : n); }
	__forceinline float32_t Mix(float32_t x, float32_t y, float32_t n) { return x * (1.0f - n) + y * n; }
	__forceinline float32_t Step(float32_t edge, float32_t x) { return (x < edge ? 0.0f : 1.0f); }
	__forceinline float32_t SmootStep(float32_t edge0, float32_t edge1, float32_t x) 
	{
		if (x < edge0) return 0.0f;
		if (x > edge1) return 1.0f;
		float32_t t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
		return t * t * (3.0f - 2.0f * t);
	}
	
	struct Vec2
	{
		float32_t x, y;

		Vec2() : x(0.0f), y(0.0f){}
		Vec2(float32_t x) : x(x), y(x) {}
		Vec2(float32_t x, float32_t y) : x(x), y(y) {}
		Vec2& operator=(const float32_t& n) { x = n; y = n; return *this; }
		Vec2& operator=(const Vec2& n) { x = n.x; y = n.y; return *this; }
		bool operator==(const Vec2& n) const { return x == n.x && y == n.y; }
		Vec2& operator+=(const float& n) { x += n; y += n; return *this; }
		Vec2& operator-=(const float& n) { x -= n; y -= n; return *this; }
		Vec2& operator*=(const float& n) { x *= n; y *= n; return *this; }
		Vec2& operator/=(const float& n) { x /= n; y /= n; return *this; }
		Vec2& operator+=(const Vec2& n) { x += n.x; y += n.y; return *this; }
		Vec2& operator-=(const Vec2& n) { x -= n.x; y -= n.y; return *this; }
		Vec2& operator*=(const Vec2& n) { x *= n.x; y *= n.y; return *this; }
		Vec2& operator/=(const Vec2& n) { x /= n.x; y /= n.y; return *this; }
		float lengthSqr() const { return x * x + y * y; }
		float length() const { return sqrtf(lengthSqr()); }
		float dot(const Vec2& n) const { return x * n.x + y * n.y; }
		float distance(const Vec2& n) const { return (*this - n).length(); }
		Vec2& normalize()
		{
			float32_t len = length();
			if (len != 0.0f)
			{
				x /= len;
				y /= len;
			}
			return *this;
		}
		Vec2& faceforward(const Vec2& incident, const Vec2& reference)
		{
			if (incident.dot(reference) >= 0.0f)
			{
				*this *= -1.0f;
			}
			return *this;
		}
		Vec2& reflect(const Vec2& normal)
		{
			*this -= 2.0f * dot(normal) * normal;
			return *this;
		}
		Vec2& refract(const Vec2& normal, float32_t indexOfRefraction)
		{
			Vec2 incident = *this;
			float32_t IdotN = incident.dot(normal);
			float32_t k = 1.0f - indexOfRefraction * indexOfRefraction * (1.0f - IdotN * IdotN);
			if (k < 0.0f) *this = 0.0f;
			else *this = indexOfRefraction * incident - (indexOfRefraction * IdotN + Sqrt(k)) * normal;
			return *this;
		}
		Vec2& operator*=(const Mat2& m);
		Vec3 toVec3();
		Vec4 toVec4();
	};

	struct Vec3
	{
		float32_t x, y, z;
	
		Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vec3(float32_t x) : x(x), y(x), z(x) {}
		Vec3(float32_t x, float32_t y, float32_t z) : x(x), y(y), z(z) {}
		Vec3& operator=(const float32_t& n) { x = n; y = n; z = n; return *this; }
		Vec3& operator=(const Vec3& n) { x = n.x; y = n.y; z = n.z; return *this; }
		bool operator==(const Vec3& n) const { return x == n.x && y == n.y && z == n.z; }
		Vec3& operator+=(const float& n) { x += n; y += n; z += n; return *this; }
		Vec3& operator-=(const float& n) { x -= n; y -= n; z -= n; return *this; }
		Vec3& operator*=(const float& n) { x *= n; y *= n; z *= n; return *this; }
		Vec3& operator/=(const float& n) { x /= n; y /= n; z /= n; return *this; }
		Vec3& operator+=(const Vec3& n) { x += n.x; y += n.y; z += n.z; return *this; }
		Vec3& operator-=(const Vec3& n) { x -= n.x; y -= n.y; z -= n.z; return *this; }
		Vec3& operator*=(const Vec3& n) { x *= n.x; y *= n.y; z *= n.z; return *this; }
		Vec3& operator/=(const Vec3& n) { x /= n.x; y /= n.y; z /= n.z; return *this; }
		float lengthSqr() const { return x * x + y * y + z * z; }
		float length() const { return sqrtf(lengthSqr()); }
		float dot(const Vec3& n) const { return x * n.x + y * n.y + z * n.z; }
		float distance(const Vec3& n) const { return (*this - n).length(); }
		Vec3& normalize()
		{
			float32_t len = length();
			if (len != 0.0f)
			{
				x /= len;
				y /= len;
				z /= len;
			}
			return *this;
		}
		Vec3& cross(const Vec3& n)
		{
			float32_t cx = y * n.z - n.y * z;
			float32_t cy = z * n.x - n.z * x;
			float32_t cz = x * n.y - n.x * y;
			x = cx;
			y = cy;
			z = cz;
			return *this;
		}
		Vec3& faceforward(const Vec3& incident, const Vec3& reference)
		{
			if (incident.dot(reference) >= 0.0f)
			{
				*this *= -1.0f;
			}
			return *this;
		}
		Vec3& reflect(const Vec3& normal)
		{
			*this -= 2.0f * dot(normal) * normal;
			return *this;
		}
		Vec3& refract(const Vec3& normal, float32_t indexOfRefraction)
		{
			Vec3 incident = *this;
			float32_t IdotN = incident.dot(normal);
			float32_t k = 1.0f - indexOfRefraction * indexOfRefraction * (1.0f - IdotN * IdotN);
			if (k < 0.0f) *this = 0.0f;
			else *this = indexOfRefraction * incident - (indexOfRefraction * IdotN + Sqrt(k)) * normal;
			return *this;
		}
		Vec3& operator*=(const Mat3& m);
		Vec4 toVec4();
		Vec2 toVec2() { return Vec2(x, y); }
	};

	struct Vec4
	{
		float32_t x, y, z, w;

		Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Vec4(float32_t x) : x(x), y(x), z(x), w(x) {}
		Vec4(float32_t x, float32_t y, float32_t z, float32_t w) : x(x), y(y), z(z), w(w) {}
		Vec4& operator=(const float32_t& n) { x = n; y = n; z = n; w = n; return *this; }
		Vec4& operator=(const Vec2& n) { x = n.x; y = n.y; return *this; }
		bool operator==(const Vec4& n) const { return x == n.x && y == n.y && z == n.z && w == n.w; }
		Vec4& operator+=(const float& n) { x += n; y += n; z += n; w += n; return *this; }
		Vec4& operator-=(const float& n) { x -= n; y -= n; z -= n; w -= n; return *this; }
		Vec4& operator*=(const float& n) { x *= n; y *= n; z *= n; w *= n; return *this; }
		Vec4& operator/=(const float& n) { x /= n; y /= n; z /= n; w /= n; return *this; }
		Vec4& operator+=(const Vec4& n) { x += n.x; y += n.y; z += n.z; w += n.w; return *this; }
		Vec4& operator-=(const Vec4& n) { x -= n.x; y -= n.y; z -= n.z; w -= n.w; return *this; }
		Vec4& operator*=(const Vec4& n) { x *= n.x; y *= n.y; z *= n.z; w *= n.w; return *this; }
		Vec4& operator/=(const Vec4& n) { x /= n.x; y /= n.y; z /= n.z; w /= n.w; return *this; }
		float lengthSqr() const { return x * x + y * y + z * z + w * w; }
		float length() const { return sqrtf(lengthSqr()); }
		float dot(const Vec4& n) const { return x * n.x + y * n.y + z * n.z + w * n.w; }
		float distance(const Vec4& n) const { return (*this - n).length(); }
		Vec4& normalize()
		{
			float32_t len = length();
			if (len != 0.0f)
			{
				x /= len;
				y /= len;
				z /= len;
				w /= len;
			}
			return *this;
		}
		Vec4& faceforward(const Vec4& incident, const Vec4& reference)
		{
			if (incident.dot(reference) >= 0.0f)
			{
				*this *= -1.0f;
			}
			return *this;
		}
		Vec4& reflect(const Vec4& normal)
		{
			*this -= 2.0f * dot(normal) * normal;
			return *this;
		}
		Vec4& refract(const Vec4& normal, float32_t indexOfRefraction)
		{
			Vec4 incident = *this;
			float32_t IdotN = incident.dot(normal);
			float32_t k = 1.0f - indexOfRefraction * indexOfRefraction * (1.0f - IdotN * IdotN);
			if (k < 0.0f) *this = 0.0f;
			else *this = indexOfRefraction * incident - (indexOfRefraction * IdotN + Sqrt(k)) * normal;
			return *this;
		}
		Vec4& operator*=(const Mat4& m);
		Vec3 toVec3() { return Vec3(x, y, z); }
		Vec2 toVec2() { return Vec2(x, y); }
	};
	
	struct Mat2
	{
		float32_t matrix[4];
		Mat2()
		{
			matrix[0] = 1.0f; matrix[1] = 0.0f;
			matrix[2] = 0.0f; matrix[3] = 1.0f;
		}
		Mat2(float32_t a, float32_t b, float32_t c, float32_t d)
		{
			matrix[0] = a; matrix[1] = b;
			matrix[2] = c; matrix[3] = d;
		}
		void setIdentity()
		{
			matrix[0] = 1.0f; matrix[1] = 0.0f;
			matrix[2] = 0.0f; matrix[3] = 1.0f;
		}
		Mat2& operator=(const Mat2& n) 
		{ 
			matrix[0] = n.matrix[0]; matrix[1] = n.matrix[1];
			matrix[2] = n.matrix[2]; matrix[3] = n.matrix[3];
			return *this; 
		}
		bool operator==(const Mat2& n) const
		{
			return matrix[0] == n.matrix[0] && matrix[1] == n.matrix[1] &&
			matrix[2] == n.matrix[2] && matrix[3] == n.matrix[3];
		}
		Mat2& transpose()
		{
			float32_t m = matrix[1];
			matrix[1] = matrix[2];
			matrix[2] = m;
			return *this;
		}
		Mat2& operator*=(const Mat2& m)
		{
			float32_t a00 = matrix[0]; float32_t a01 = matrix[1];;
			float32_t a10 = matrix[2]; float32_t a11 = matrix[3];;

			float32_t b0 = m.matrix[0];
			float32_t b1 = m.matrix[1];

			matrix[0] = b0 * a00 + b1 * a10;
			matrix[1] = b0 * a01 + b1 * a11;

			b0 = m.matrix[2];
			b1 = m.matrix[3];

			matrix[2] = b0 * a00 + b1 * a10;
			matrix[3] = b0 * a01 + b1 * a11;

			return *this;
		}
		Mat2& rotate(float32_t rad)
		{
			float32_t c = Cos(rad);
			float32_t s = Sin(rad);
			float32_t m00 = matrix[0]; float32_t m01 = matrix[1];
			float32_t m10 = matrix[2]; float32_t m11 = matrix[3];

			matrix[0] = m00 * c + m10 * s;
			matrix[1] = m01 * c + m11 * s;
			matrix[2] = m00 * -s + m10 * c;
			matrix[3] = m01 * -s + m11 * c;

			return *this;
		}
		Mat2& scale(const Vec2& v)
		{
			matrix[0] = v.x * matrix[0];
			matrix[1] = v.x * matrix[1];
			matrix[2] = v.y * matrix[2];
			matrix[3] = v.y * matrix[3];
			return *this;
		}
	};

	struct Mat3
	{
		float32_t matrix[9];
		Mat3()
		{
			matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f;
			matrix[3] = 0.0f; matrix[4] = 1.0f; matrix[5] = 0.0f;
			matrix[6] = 0.0f; matrix[7] = 0.0f; matrix[8] = 1.0f;
		}
		Mat3(float32_t a, float32_t b, float32_t c, 
			float32_t d, float32_t e, float32_t f,
			float32_t g, float32_t h, float32_t i)
		{
			matrix[0] = a; matrix[1] = b; matrix[2] = c;
			matrix[3] = d; matrix[4] = e; matrix[5] = f;
			matrix[6] = g; matrix[7] = h; matrix[8] = i;
		}
		void setIdentity()
		{
			matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f;
			matrix[3] = 0.0f; matrix[4] = 1.0f; matrix[5] = 0.0f;
			matrix[6] = 0.0f; matrix[7] = 0.0f; matrix[8] = 1.0f;
		}
		Mat3& operator=(const Mat3& n)
		{
			matrix[0] = n.matrix[0]; matrix[1] = n.matrix[1]; matrix[2] = n.matrix[2];
			matrix[3] = n.matrix[3]; matrix[4] = n.matrix[4]; matrix[5] = n.matrix[5];
			matrix[6] = n.matrix[6]; matrix[7] = n.matrix[7]; matrix[8] = n.matrix[8];
			return *this;
		}
		bool operator==(const Mat3& n) const
		{
			return matrix[0] == n.matrix[0] && matrix[1] == n.matrix[1] && matrix[2] == n.matrix[2] &&
			matrix[3] == n.matrix[3] && matrix[4] == n.matrix[4] && matrix[5] == n.matrix[5];
			matrix[6] == n.matrix[6] && matrix[7] == n.matrix[7] && matrix[8] == n.matrix[8];
		}
		Mat3& transpose()
		{
			float32_t m1 = matrix[1];
			float32_t m2 = matrix[2];
			float32_t m5 = matrix[5];
			matrix[1] = matrix[3];
			matrix[2] = matrix[6];
			matrix[3] = m1;
			matrix[5] = matrix[7];
			matrix[6] = m2;
			matrix[7] = m5;
			return *this;
		}
		Mat3& operator*=(const Mat3& m)
		{
			float32_t a00 = matrix[0]; float32_t a01 = matrix[1]; float32_t a02 = matrix[2];
			float32_t a10 = matrix[3]; float32_t a11 = matrix[4]; float32_t a12 = matrix[5];
			float32_t a20 = matrix[6]; float32_t a21 = matrix[7]; float32_t a22 = matrix[8];

			float32_t b0 = m.matrix[0];
			float32_t b1 = m.matrix[1];
			float32_t b2 = m.matrix[2];

			matrix[0] = b0 * a00 + b1 * a10 + b2 * a20;
			matrix[1] = b0 * a01 + b1 * a11 + b2 * a21;
			matrix[2] = b0 * a02 + b1 * a12 + b2 * a22;

			b0 = m.matrix[3];
			b1 = m.matrix[4];
			b2 = m.matrix[5];

			matrix[3] = b0 * a00 + b1 * a10 + b2 * a20;
			matrix[4] = b0 * a01 + b1 * a11 + b2 * a21;
			matrix[5] = b0 * a02 + b1 * a12 + b2 * a22;

			b0 = m.matrix[6];
			b1 = m.matrix[7];
			b2 = m.matrix[8];

			matrix[6] = b0 * a00 + b1 * a10 + b2 * a20; 
			matrix[7] = b0 * a01 + b1 * a11 + b2 * a21; 
			matrix[8] = b0 * a02 + b1 * a12 + b2 * a22;

			return *this;
		}

		Mat3& translate(const Vec3& n)
		{
			matrix[6] = n.x * matrix[0] + matrix[3];
			matrix[7] = n.x * matrix[0] + matrix[3];
			matrix[8] = n.x * matrix[0] + matrix[3];
			return *this;
		}

		Mat3& rotate(float32_t rad)
		{
			float32_t c = Cos(rad);
			float32_t s = Sin(rad);
			float32_t a00 = matrix[0]; float32_t a01 = matrix[1]; float32_t a02 = matrix[2];
			float32_t a10 = matrix[3]; float32_t a11 = matrix[4]; float32_t a12 = matrix[5];
			matrix[0] = c * a00 + s * a10;
			matrix[1] = c * a01 + s * a11;
			matrix[2] = c * a02 + s * a12;
			matrix[3] = -s * a00 + c * a10;
			matrix[4] = -s * a01 + c * a11;
			matrix[5] = -s * a02 + c * a12;
			return *this;
		}
		Mat3& scale(const Vec2& n)
		{
			matrix[0] = n.x * matrix[0];
			matrix[1] = n.x * matrix[1];
			matrix[2] = n.x * matrix[2];
			matrix[3] = n.y * matrix[3];
			matrix[4] = n.y * matrix[4];
			matrix[5] = n.y * matrix[5];
			return *this;
		}
	};

	struct Mat4
	{
		float32_t matrix[16];
		Mat4()
		{
			matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f; matrix[3] = 0.0f;
			matrix[4] = 0.0f; matrix[5] = 1.0f; matrix[6] = 0.0f; matrix[7] = 0.0f;
			matrix[8] = 0.0f; matrix[9] = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
			matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
		}
		Mat4(float32_t a, float32_t b, float32_t c, float32_t d, 
			float32_t e, float32_t f, float32_t g, float32_t h, 
			float32_t i, float32_t j, float32_t k, float32_t l,
			float32_t m, float32_t n, float32_t o, float32_t p)
		{
			matrix[0] = a; matrix[1] = b; matrix[2] = c; matrix[3] = d; 
			matrix[4] = e; matrix[5] = f; matrix[6] = g; matrix[7] = h; 
			matrix[8] = i; matrix[9] = j; matrix[10] = k; matrix[11] = l;
			matrix[12] = m; matrix[13] = n; matrix[14] = o; matrix[15] = p;
		}
		void setIdentity()
		{
			matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f; matrix[3] = 0.0f;
			matrix[4] = 0.0f; matrix[5] = 1.0f; matrix[6] = 0.0f; matrix[7] = 0.0f;
			matrix[8] = 0.0f; matrix[9] = 0.0f; matrix[10] = 1.0f; matrix[11] = 0.0f;
			matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
		}
		Mat4& operator=(const Mat4& n)
		{
			matrix[0] = n.matrix[0]; matrix[1] = n.matrix[1]; matrix[2] = n.matrix[2]; matrix[3] = n.matrix[3]; 
			matrix[4] = n.matrix[4]; matrix[5] = n.matrix[5]; matrix[6] = n.matrix[6]; matrix[7] = n.matrix[7]; 
			matrix[8] = n.matrix[8]; matrix[9] = n.matrix[9]; matrix[10] = n.matrix[10]; matrix[11] = n.matrix[11];
			matrix[12] = n.matrix[12]; matrix[13] = n.matrix[13]; matrix[14] = n.matrix[14]; matrix[15] = n.matrix[15];
			return *this;
		}
		bool operator==(const Mat4& n) const
		{
			return matrix[0] == n.matrix[0] && matrix[1] == n.matrix[1] && matrix[2] == n.matrix[2] && matrix[3] == n.matrix[3] &&
			matrix[4] == n.matrix[4] && matrix[5] == n.matrix[5] && matrix[6] == n.matrix[6] && matrix[7] == n.matrix[7] &&
			matrix[8] == n.matrix[8] && matrix[9] == n.matrix[9] && matrix[10] == n.matrix[10] && matrix[11] == n.matrix[11] &&
			matrix[12] == n.matrix[12] && matrix[13] == n.matrix[13] && matrix[14] == n.matrix[14] && matrix[15] == n.matrix[15];
		}
		Mat4& transpose()
		{
			float32_t m00 = matrix[0x0]; float32_t m01 = matrix[0x1]; float32_t m02 = matrix[0x2]; float32_t m03 = matrix[0x3];
			float32_t m10 = matrix[0x4]; float32_t m11 = matrix[0x5]; float32_t m12 = matrix[0x6]; float32_t m13 = matrix[0x7];
			float32_t m20 = matrix[0x8]; float32_t m21 = matrix[0x9]; float32_t m22 = matrix[0xA]; float32_t m23 = matrix[0xB];
			float32_t m30 = matrix[0xC]; float32_t m31 = matrix[0xD]; float32_t m32 = matrix[0xE]; float32_t m33 = matrix[0xF];

			matrix[0x0] = m00; matrix[0x1] = m10; matrix[0x2] = m20; matrix[0x3] = m30;
			matrix[0x4] = m01; matrix[0x5] = m11; matrix[0x6] = m21; matrix[0x7] = m31;
			matrix[0x8] = m02; matrix[0x9] = m12; matrix[0xA] = m22; matrix[0xB] = m32;
			matrix[0xC] = m03; matrix[0xD] = m13; matrix[0xE] = m23; matrix[0xF] = m33;
			return *this;
		}
		Mat4& operator*=(const Mat4& m)
		{
			float32_t a00 = matrix[0]; float32_t a01 = matrix[1]; float32_t a02 = matrix[2]; float32_t a03 = matrix[3];
			float32_t a10 = matrix[4]; float32_t a11 = matrix[5]; float32_t a12 = matrix[6]; float32_t a13 = matrix[7];
			float32_t a20 = matrix[8]; float32_t a21 = matrix[9]; float32_t a22 = matrix[10]; float32_t a23 = matrix[11];
			float32_t a30 = matrix[12]; float32_t a31 = matrix[13]; float32_t a32 = matrix[14]; float32_t a33 = matrix[15];

			float32_t b0 = m.matrix[0];
			float32_t b1 = m.matrix[1];
			float32_t b2 = m.matrix[2];
			float32_t b3 = m.matrix[3];

			matrix[0] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
			matrix[1] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
			matrix[2] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
			matrix[3] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

			b0 = m.matrix[4];
			b1 = m.matrix[5];
			b2 = m.matrix[6];
			b3 = m.matrix[7];

			matrix[4] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
			matrix[5] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
			matrix[6] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
			matrix[7] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

			b0 = m.matrix[8];
			b1 = m.matrix[9];
			b2 = m.matrix[10];
			b3 = m.matrix[11];

			matrix[8] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
			matrix[9] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
			matrix[10] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
			matrix[11] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

			b0 = m.matrix[12];
			b1 = m.matrix[13];
			b2 = m.matrix[14];
			b3 = m.matrix[15];

			matrix[12] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
			matrix[13] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
			matrix[14] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
			matrix[15] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

			return *this;
		}
		Mat4& translate(const Vec3& v)
		{
			matrix[12] = matrix[0] * v.x + matrix[4] * v.y + matrix[8] * v.z + matrix[12];
			matrix[13] = matrix[1] * v.x + matrix[5] * v.y + matrix[9] * v.z + matrix[13];
			matrix[14] = matrix[2] * v.x + matrix[6] * v.y + matrix[10] * v.z + matrix[14];
			matrix[15] = matrix[3] * v.x + matrix[7] * v.y + matrix[11] * v.z + matrix[15];
			return *this;
		}
		Mat4& scale(const Vec3& v)
		{
			matrix[0] = matrix[0] * v.x;
			matrix[1] = matrix[1] * v.x;
			matrix[2] = matrix[2] * v.x;
			matrix[3] = matrix[3] * v.x;
			matrix[4] = matrix[4] * v.y;
			matrix[5] = matrix[5] * v.y;
			matrix[6] = matrix[6] * v.y;
			matrix[7] = matrix[7] * v.y;
			matrix[8] = matrix[8] * v.z;
			matrix[9] = matrix[9] * v.z;
			matrix[10] = matrix[10] * v.z;
			matrix[11] = matrix[11] * v.z;
			return *this;
		}
		Mat4& rotateX(float32_t rad)
		{
			float32_t s = Sin(rad);
			float32_t c = Cos(rad);
			float32_t a10 = matrix[4];
			float32_t a11 = matrix[5];
			float32_t a12 = matrix[6];
			float32_t a13 = matrix[7];
			float32_t a20 = matrix[8];
			float32_t a21 = matrix[9];
			float32_t a22 = matrix[10];
			float32_t a23 = matrix[11];
			matrix[4] = a10 * c + a20 * s;
			matrix[5] = a11 * c + a21 * s;
			matrix[6] = a12 * c + a22 * s;
			matrix[7] = a13 * c + a23 * s;
			matrix[8] = a20 * c - a10 * s;
			matrix[9] = a21 * c - a11 * s;
			matrix[10] = a22 * c - a12 * s;
			matrix[11] = a23 * c - a13 * s;
			return *this;
		}
		Mat4& rotateY(float32_t rad)
		{
			float32_t s = Sin(rad);
			float32_t c = Cos(rad);
			float32_t a00 = matrix[0];
			float32_t a01 = matrix[1];
			float32_t a02 = matrix[2];
			float32_t a03 = matrix[3];
			float32_t a20 = matrix[8];
			float32_t a21 = matrix[9];
			float32_t a22 = matrix[10];
			float32_t a23 = matrix[11];
			
			matrix[0] = a00 * c - a20 * s;
			matrix[1] = a01 * c - a21 * s;
			matrix[2] = a02 * c - a22 * s;
			matrix[3] = a03 * c - a23 * s;
			matrix[8] = a00 * s + a20 * c;
			matrix[9] = a01 * s + a21 * c;
			matrix[10] = a02 * s + a22 * c;
			matrix[11] = a03 * s + a23 * c;
			return *this;
		}
		Mat4& rotateZ(float32_t rad)
		{
			float32_t s = Sin(rad);
			float32_t c = Cos(rad);
			float32_t a00 = matrix[0];
			float32_t a01 = matrix[1];
			float32_t a02 = matrix[2];
			float32_t a03 = matrix[3];
			float32_t a10 = matrix[4];
			float32_t a11 = matrix[5];
			float32_t a12 = matrix[6];
			float32_t a13 = matrix[7];

			matrix[0] = a00 * c + a10 * s;
			matrix[1] = a01 * c + a11 * s;
			matrix[2] = a02 * c + a12 * s;
			matrix[3] = a03 * c + a13 * s;
			matrix[4] = a10 * c - a00 * s;
			matrix[5] = a11 * c - a01 * s;
			matrix[6] = a12 * c - a02 * s;
			matrix[7] = a13 * c - a03 * s;
			return *this;
		}
		Mat4& perspective(float32_t fovy, float32_t aspect, float32_t nearBound, float32_t farBound)
		{
			float32_t f = 1.0f / Tan(fovy / 2.0f);
			matrix[0] = f / aspect;
			matrix[1] = 0;
			matrix[2] = 0;
			matrix[3] = 0;
			matrix[4] = 0;
			matrix[5] = f;
			matrix[6] = 0;
			matrix[7] = 0;
			matrix[8] = 0;
			matrix[9] = 0;
			matrix[11] = -1;
			matrix[12] = 0;
			matrix[13] = 0;
			matrix[15] = 0;
			float32_t nf = 1 / (nearBound - farBound);
			matrix[10] = (farBound + nearBound) * nf;
			matrix[14] = (2 * farBound * nearBound) * nf;
			return *this;
		}
		Mat4& orthographic(float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t nearBound, float32_t farBound)
		{
			float32_t lr = 1 / (left - right);
			float32_t bt = 1 / (bottom - top);
			float32_t nf = 1 / (nearBound - farBound);
			matrix[0] = -2 * lr;
			matrix[1] = 0;
			matrix[2] = 0;
			matrix[3] = 0;
			matrix[4] = 0;
			matrix[5] = -2 * bt;
			matrix[6] = 0;
			matrix[7] = 0;
			matrix[8] = 0;
			matrix[9] = 0;
			matrix[10] = 2 * nf;
			matrix[11] = 0;
			matrix[12] = (left + right) * lr;
			matrix[13] = (top + bottom) * bt;
			matrix[14] = (nearBound + farBound) * nf;
			matrix[15] = 1;
			return *this;
		}
		Mat4& lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
		{
			const float32_t E = 0.000001f;
			float32_t x0, x1, x2, y0, y1, y2, z0, z1, z2, len;
			float32_t eyex = eye.x;
			float32_t eyey = eye.y;
			float32_t eyez = eye.z;
			float32_t upx = up.x;
			float32_t upy = up.y;
			float32_t upz = up.z;
			float32_t centerx = center.x;
			float32_t centery = center.y;
			float32_t centerz = center.z;
			if (Abs(eyex - centerx) < E &&
				Abs(eyey - centery) < E &&
				Abs(eyez - centerz) < E)
			{
				setIdentity();
				return *this;
			}
			z0 = eyex - centerx;
			z1 = eyey - centery;
			z2 = eyez - centerz;
			len = 1.0f / Sqrt(z0 * z0 + z1 * z1 + z2 * z2);
			z0 *= len;
			z1 *= len;
			z2 *= len;
			x0 = upy * z2 - upz * z1;
			x1 = upz * z0 - upx * z2;
			x2 = upx * z1 - upy * z0;
			len = Sqrt(x0 * x0 + x1 * x1 + x2 * x2);
			if (!len)
			{
				x0 = 0.0f;
				x1 = 0.0f;
				x2 = 0.0f;
			}
			else
			{
				len = 1.0f / len;
				x0 *= len;
				x1 *= len;
				x2 *= len;
			}
			y0 = z1 * x2 - z2 * x1;
			y1 = z2 * x0 - z0 * x2;
			y2 = z0 * x1 - z1 * x0;
			len = Sqrt(y0 * y0 + y1 * y1 + y2 * y2);
			if (!len)
			{
				y0 = 0.0f;
				y1 = 0.0f;
				y2 = 0.0f;
			}
			else
			{
				len = 1.0f / len;
				y0 *= len;
				y1 *= len;
				y2 *= len;
			}
			matrix[0] = x0;
			matrix[1] = y0;
			matrix[2] = z0;
			matrix[3] = 0.0f;
			matrix[4] = x1;
			matrix[5] = y1;
			matrix[6] = z1;
			matrix[7] = 0.0f;
			matrix[8] = x2;
			matrix[9] = y2;
			matrix[10] = z2;
			matrix[11] = 0.0f;
			matrix[12] = -(x0 * eyex + x1 * eyey + x2 * eyez);
			matrix[13] = -(y0 * eyex + y1 * eyey + y2 * eyez);
			matrix[14] = -(z0 * eyex + z1 * eyey + z2 * eyez);
			matrix[15] = 1;
			return *this;
		}

		Mat4& invert()
		{
			float32_t d0 = matrix[0] * matrix[5] - matrix[1] * matrix[4];
			float32_t d1 = matrix[0] * matrix[6] - matrix[2] * matrix[4];
			float32_t d2 = matrix[0] * matrix[7] - matrix[3] * matrix[4];
			float32_t d3 = matrix[1] * matrix[6] - matrix[2] * matrix[5];
			float32_t d4 = matrix[1] * matrix[7] - matrix[3] * matrix[5];
			float32_t d5 = matrix[2] * matrix[7] - matrix[3] * matrix[6];
			float32_t d6 = matrix[8] * matrix[13] - matrix[9] * matrix[12];
			float32_t d7 = matrix[8] * matrix[14] - matrix[10] * matrix[12];
			float32_t d8 = matrix[8] * matrix[15] - matrix[11] * matrix[12];
			float32_t d9 = matrix[9] * matrix[14] - matrix[10] * matrix[13];
			float32_t d10 = matrix[9] * matrix[15] - matrix[11] * matrix[13];
			float32_t d11 = matrix[10] * matrix[15] - matrix[11] * matrix[14];
			float32_t determinant = d0 * d11 - d1 * d10 + d2 * d9 + d3 * d8 - d4 * d7 + d5 * d6;

			if (determinant == 0.0f) return *this;

			determinant = 1.0f / determinant;
			float32_t m00 = (matrix[5] * d11 - matrix[6] * d10 + matrix[7] * d9) * determinant;
			float32_t m01 = (matrix[2] * d10 - matrix[1] * d11 - matrix[3] * d9) * determinant;
			float32_t m02 = (matrix[13] * d5 - matrix[14] * d4 + matrix[15] * d3) * determinant;
			float32_t m03 = (matrix[10] * d4 - matrix[9] * d5 - matrix[11] * d3) * determinant;
			float32_t m04 = (matrix[6] * d8 - matrix[4] * d11 - matrix[7] * d7) * determinant;
			float32_t m05 = (matrix[0] * d11 - matrix[2] * d8 + matrix[3] * d7) * determinant;
			float32_t m06 = (matrix[14] * d2 - matrix[12] * d5 - matrix[15] * d1) * determinant;
			float32_t m07 = (matrix[8] * d5 - matrix[10] * d2 + matrix[11] * d1) * determinant;
			float32_t m08 = (matrix[4] * d10 - matrix[5] * d8 + matrix[7] * d6) * determinant;
			float32_t m09 = (matrix[1] * d8 - matrix[0] * d10 - matrix[3] * d6) * determinant;
			float32_t m10 = (matrix[12] * d4 - matrix[13] * d2 + matrix[15] * d0) * determinant;
			float32_t m11 = (matrix[9] * d2 - matrix[8] * d4 - matrix[11] * d0) * determinant;
			float32_t m12 = (matrix[5] * d7 - matrix[4] * d9 - matrix[6] * d6) * determinant;
			float32_t m13 = (matrix[0] * d9 - matrix[1] * d7 + matrix[2] * d6) * determinant;
			float32_t m14 = (matrix[13] * d1 - matrix[12] * d3 - matrix[14] * d0) * determinant;
			float32_t m15 = (matrix[8] * d3 - matrix[9] * d1 + matrix[10] * d0) * determinant;
			
			matrix[0] = m00;
			matrix[1] = m01;
			matrix[2] = m02;
			matrix[3] = m03;
			matrix[4] = m04;
			matrix[5] = m05;
			matrix[6] = m06;
			matrix[7] = m07;
			matrix[8] = m08;
			matrix[9] = m09;
			matrix[10] = m10;
			matrix[11] = m11;
			matrix[12] = m12;
			matrix[13] = m13;
			matrix[14] = m14;
			matrix[15] = m15;

			return *this;
		}
	};


	struct Quaternion
	{
		float32_t x, y, z, w;

		Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		Quaternion(float32_t x) : x(x), y(x), z(x), w(x) {}
		Quaternion(float32_t x, float32_t y, float32_t z, float32_t w) : x(x), y(y), z(z), w(w) {}
		void setIdentity() { x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f; }
		Quaternion& fromEulerAngles(const Vec3& eulerAngles)
		{
			float32_t roll = eulerAngles.x;
			float32_t pitch = eulerAngles.y;
			float32_t yaw = eulerAngles.z;
			float32_t cy = Cos(yaw * 0.5f);
			float32_t sy = Sin(yaw * 0.5f);
			float32_t cp = Cos(pitch * 0.5f);
			float32_t sp = Sin(pitch * 0.5f);
			float32_t cr = Cos(roll * 0.5f);
			float32_t sr = Sin(roll * 0.5f);
			
			w = cy * cp * cr + sy * sp * sr;
			x = cy * cp * sr + sy * sp * cr;
			y = sy * cp * sr + cy * sp * cr;
			z = sy * cp * cr + cy * sp * sr;
			
			return *this;
		}
		Vec3 toEulerAngles()
		{
			float32_t roll, pitch, yaw;
			float32_t a = 2.0f * (w * x + y * z);
			float32_t b = 1.0f - 2.0f * (x * x + y * y);
			
			roll = Atan2(a, b);

			float32_t c = 2.0f * (w * y * z * x);
			if (Abs(c) >= 1.0f)
				pitch = copysignf(PI / 2.0f, c);
			else
				pitch = Asin(c);
		
			float32_t d = 2.0f * (w * z + x * y);
			float32_t e = 1.0f - 2.0f * (y * y + z * z);
			yaw = Atan2(d, e);

			return Vec3(roll, pitch, yaw);
		}

		Quaternion& rotateX(float32_t rad)
		{
			float32_t c = Cos(rad);
			float32_t s = Sin(rad);
			float32_t tx = x * s + w * c;
			float32_t ty = y * s + z * c;
			float32_t tz = z * s - y * c;
			float32_t tw = w * s - x * c;
			x = tx;
			y = ty;
			z = tz;
			w = tw;
			return *this;
		}

		Quaternion& rotateY(float32_t rad)
		{
			float32_t c = Cos(rad);
			float32_t s = Sin(rad);
			float32_t tx = x * s - z * c;
			float32_t ty = y * s + w * c;
			float32_t tz = z * s + x * c;
			float32_t tw = w * s - y * c;
			x = tx;
			y = ty;
			z = tz;
			w = tw;
			return *this;
		}

		Quaternion& rotateZ(float32_t rad)
		{
			float32_t c = Cos(rad);
			float32_t s = Sin(rad);
			float32_t tx = x * s + y * c;
			float32_t ty = y * s - x * c;
			float32_t tz = z * s + w * c;
			float32_t tw = w * s - z * c;
			x = tx;
			y = ty;
			z = tz;
			w = tw;
			return *this;
		}

		Mat4 toMat4()
		{
			float32_t x2 = x + x;
			float32_t y2 = y + y;
			float32_t z2 = z + z;
			float32_t xx = x * x2;
			float32_t yx = y * x2;
			float32_t yy = y * y2;
			float32_t zx = z * x2;
			float32_t zy = z * y2;
			float32_t zz = z * z2;
			float32_t wx = w * x2;
			float32_t wy = w * y2;
			float32_t wz = w * z2;

			return Mat4(
				1.0f - yy - zz,
				yx + wz,
				zx - wy,
				0.0f,
				yx - wz,
				1.0f - xx - zz,
				zy + wx,
				0.0f,
				zx + wy,
				zy - wx,
				1.0f - xx - yy,
				0.0f,
				0.0f,
				0.0f,
				0.0f,
				1.0f
			);
		}
	};

	struct Transform
	{
		Transform() : position(0.0f), scale(1.0f), rotation() {}
		Vec3 position;
		Vec3 scale;
		Quaternion rotation;
	};
}

