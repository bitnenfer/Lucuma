#include "math.h"

namespace lu
{
	// Vec2
	Vec3 Vec2::toVec3() { return Vec3(x, y, 0.0f); }
	Vec4 Vec2::toVec4() { return Vec4(x, y, 0.0f, 0.0f); }
	Vec2& Vec2::operator*=(const Mat2& m)
	{
		float32_t tx = m.matrix[0] * x + m.matrix[2] * y;
		float32_t ty = m.matrix[1] * x + m.matrix[3] * y;
		x = tx;
		y = ty;
		return *this;
	}

	// Vec3
	Vec4 Vec3::toVec4() { return Vec4(x, y, z, 0.0f); }
	Vec3& Vec3::operator*=(const Mat3& m)
	{
		float32_t tx = x * m.matrix[0] + y * m.matrix[3] + z * m.matrix[6];
		float32_t ty = x * m.matrix[1] + y * m.matrix[4] + z * m.matrix[7];
		float32_t tz = x * m.matrix[2] + y * m.matrix[5] + z * m.matrix[8];
		x = tx;
		y = ty;
		z = tz;
		return *this;
	}

	// Vec4
	Vec4& Vec4::operator*=(const Mat4& m)
	{
		float32_t tx = x * m.matrix[0x0] + y * m.matrix[0x4] + z * m.matrix[0x8] + w * m.matrix[0xC];
		float32_t ty = x * m.matrix[0x1] + y * m.matrix[0x5] + z * m.matrix[0x9] + w * m.matrix[0xD];
		float32_t tz = x * m.matrix[0x2] + y * m.matrix[0x6] + z * m.matrix[0xA] + w * m.matrix[0xE];
		float32_t tw = x * m.matrix[0x3] + y * m.matrix[0x7] + z * m.matrix[0xB] + w * m.matrix[0xF];
		x = tx;
		y = ty;
		z = tz;
		w = tw;
		return *this;
	}
}

// Global Vector Math
// Vec2

lu::Vec2 operator+(const lu::Vec2& a, const lu::Vec2& b)
{
	lu::Vec2 c = a;
	c += b;
	return c;
}

lu::Vec2 operator-(const lu::Vec2& a, const lu::Vec2& b)
{
	lu::Vec2 c = a;
	c -= b;
	return c;
}

lu::Vec2 operator*(const lu::Vec2& a, const lu::Vec2& b)
{
	lu::Vec2 c = a;
	c *= b;
	return c;
}

lu::Vec2 operator/(const lu::Vec2& a, const lu::Vec2& b)
{
	lu::Vec2 c = a;
	c /= b;
	return c;
}

lu::Vec2 operator*(const lu::Mat2& a, const lu::Vec2& b)
{
	lu::Vec2 c = b;
	c *= a;
	return c;
}

lu::Vec2 operator+(const lu::Vec2& a, const float32_t& b)
{
	lu::Vec2 c = a;
	c += b;
	return c;
}

lu::Vec2 operator-(const lu::Vec2& a, const float32_t& b)
{
	lu::Vec2 c = a;
	c -= b;
	return c;
}

lu::Vec2 operator*(const lu::Vec2& a, const float32_t& b)
{
	lu::Vec2 c = a;
	c *= b;
	return c;
}

lu::Vec2 operator/(const lu::Vec2& a, const float32_t& b)
{
	lu::Vec2 c = a;
	c /= b;
	return c;
}

lu::Vec2 operator+(const float32_t& a, const lu::Vec2& b)
{
	lu::Vec2 c = b;
	c += a;
	return c;
}

lu::Vec2 operator-(const float32_t& a, const lu::Vec2& b)
{
	lu::Vec2 c = b;
	c -= a;
	return c;
}

lu::Vec2 operator*(const float32_t& a, const lu::Vec2& b)
{
	lu::Vec2 c = b;
	c *= a;
	return c;
}

lu::Vec2 operator/(const float32_t& a, const lu::Vec2& b)
{
	lu::Vec2 c = b;
	c /= a;
	return c;
}

lu::Vec2 operator-(const lu::Vec2& a)
{
	return lu::Vec2(-a.x, -a.y);
}

// Vec3
lu::Vec3 operator+(const lu::Vec3& a, const lu::Vec3& b)
{
	lu::Vec3 c = a;
	c += b;
	return c;
}

lu::Vec3 operator-(const lu::Vec3& a, const lu::Vec3& b)
{
	lu::Vec3 c = a;
	c -= b;
	return c;
}

lu::Vec3 operator*(const lu::Vec3& a, const lu::Vec3& b)
{
	lu::Vec3 c = a;
	c *= b;
	return c;
}

lu::Vec3 operator/(const lu::Vec3& a, const lu::Vec3& b)
{
	lu::Vec3 c = a;
	c /= b;
	return c;
}

lu::Vec3 operator*(const lu::Mat3& a, const lu::Vec3& b)
{
	lu::Vec3 c = b;
	c *= a;
	return c;
}

lu::Vec3 operator+(const lu::Vec3& a, const float32_t& b)
{
	lu::Vec3 c = a;
	c += b;
	return c;
}

lu::Vec3 operator-(const lu::Vec3& a, const float32_t& b)
{
	lu::Vec3 c = a;
	c -= b;
	return c;
}

lu::Vec3 operator*(const lu::Vec3& a, const float32_t& b)
{
	lu::Vec3 c = a;
	c *= b;
	return c;
}

lu::Vec3 operator/(const lu::Vec3& a, const float32_t& b)
{
	lu::Vec3 c = a;
	c /= b;
	return c;
}

lu::Vec3 operator+(const float32_t& a, const lu::Vec3& b)
{
	lu::Vec3 c = b;
	c += a;
	return c;
}

lu::Vec3 operator-(const float32_t& a, const lu::Vec3& b)
{
	lu::Vec3 c = b;
	c -= a;
	return c;
}

lu::Vec3 operator*(const float32_t& a, const lu::Vec3& b)
{
	lu::Vec3 c = b;
	c *= a;
	return c;
}

lu::Vec3 operator/(const float32_t& a, const lu::Vec3& b)
{
	lu::Vec3 c = b;
	c /= a;
	return c;
}

lu::Vec3 operator-(const lu::Vec3& a)
{
	return lu::Vec3(-a.x, -a.y, -a.z);
}

// Vec4
lu::Vec4 operator+(const lu::Vec4& a, const lu::Vec4& b)
{
	lu::Vec4 c = a;
	c += b;
	return c;
}

lu::Vec4 operator-(const lu::Vec4& a, const lu::Vec4& b)
{
	lu::Vec4 c = a;
	c -= b;
	return c;
}

lu::Vec4 operator*(const lu::Vec4& a, const lu::Vec4& b)
{
	lu::Vec4 c = a;
	c *= b;
	return c;
}

lu::Vec4 operator/(const lu::Vec4& a, const lu::Vec4& b)
{
	lu::Vec4 c = a;
	c /= b;
	return c;
}

lu::Vec4 operator*(const lu::Mat4& a, const lu::Vec4& b)
{
	lu::Vec4 c = b;
	c *= a;
	return c;
}

lu::Vec4 operator+(const lu::Vec4& a, const float32_t& b)
{
	lu::Vec4 c = a;
	c += b;
	return c;
}

lu::Vec4 operator-(const lu::Vec4& a, const float32_t& b)
{
	lu::Vec4 c = a;
	c -= b;
	return c;
}

lu::Vec4 operator*(const lu::Vec4& a, const float32_t& b)
{
	lu::Vec4 c = a;
	c *= b;
	return c;
}

lu::Vec4 operator/(const lu::Vec4& a, const float32_t& b)
{
	lu::Vec4 c = a;
	c /= b;
	return c;
}

lu::Vec4 operator+(const float32_t& a, const lu::Vec4& b)
{
	lu::Vec4 c = b;
	c += a;
	return c;
}

lu::Vec4 operator-(const float32_t& a, const lu::Vec4& b)
{
	lu::Vec4 c = b;
	c -= a;
	return c;
}

lu::Vec4 operator*(const float32_t& a, const lu::Vec4& b)
{
	lu::Vec4 c = b;
	c *= a;
	return c;
}

lu::Vec4 operator/(const float32_t& a, const lu::Vec4& b)
{
	lu::Vec4 c = b;
	c /= a;
	return c;
}

lu::Vec4 operator-(const lu::Vec4& a)
{
	return lu::Vec4(-a.x, -a.y, -a.z, -a.w);
}

lu::Mat2 operator*(const lu::Mat2& a, const lu::Mat2& b)
{
	lu::Mat2 c = a;
	c *= b;
	return c;
}

lu::Mat3 operator*(const lu::Mat3& a, const lu::Mat3& b)
{
	lu::Mat3 c = a;
	c *= b;
	return c;
}

lu::Mat4 operator*(const lu::Mat4& a, const lu::Mat4& b)
{
	lu::Mat4 c = a;
	c *= b;
	return c;
}
