#include "pch.h"
#include "MathCore.h"


namespace BMath
{
	// -------------------------------------------------------------
	// -------------------------------------------------------------
	// --------------------   Vector 2   ---------------------------
	// -------------------------------------------------------------
	// -------------------------------------------------------------

	Vector2::Vector2(const float x, const float y) : x(x), y(y) {}



	// -------------------------------------------------------------
	// -------------------------------------------------------------
	// --------------------   Vector 3   ---------------------------
	// -------------------------------------------------------------
	// -------------------------------------------------------------
	Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

	Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

	Vector3::Vector3(Vector4 vec) : x(vec.x), y(vec.y), z(vec.z) {}

	Vector3::Vector3(const float val) : x(val), y(val), z(val) {}

	Vector3 Vector3::Forward()
	{
		Vector3 vec{};
		vec.z = -1.0f;
		return vec;
	}
	Vector3 Vector3::Right()
	{
		Vector3 vec{};
		vec.x = 1.0f;
		return vec;
	}
	Vector3 Vector3::Up()
	{
		Vector3 vec;
		vec.y = 1.0f;
		return vec;
	}

	Vector3 Vector3::Zero()
	{
		return Vector3{};
	}

	Vector3 Vector3::One()
	{
		return Vector3(1.0f, 1.0f, 1.0f);
	}


	Vector3& Vector3::Normalize()
	{
		float mag = Magnitude();

		if (mag == 0)
		{
			*this = Vector3::Zero();
			return *this;
		}

		x /= mag;
		y /= mag;
		z /= mag;

		return *this;
	}

	Vector3 Vector3::Normalize(const Vector3& vec)
	{
		const float mag = Magnitude(vec);

		if (mag == 0)
		{
			return Vector3::Zero();
		}

		return Vector3(vec.x / mag, vec.y / mag, vec.z / mag);
	}


	Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
	{
		return Vector3
		(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}

	bool Vector3::Equals(const Vector3& v1, const Vector3& v2, const float epsilon)
	{
		return  std::abs(v1.x - v2.x) <= epsilon &&
			std::abs(v1.y - v2.y) <= epsilon &&
			std::abs(v1.z - v2.z) <= epsilon;
	}

	Vector3 Vector3::Reflect(const Vector3& lightDir, const Vector3& normal)
	{
		return BMath::Vector3::Normalize(lightDir - (normal *  Vector3::Dot(normal, lightDir) * 2.0f));
	}


	// -------------------------------------------------------------
	// -------------------------------------------------------------
	// --------------------   Vector 4   ---------------------------
	// -------------------------------------------------------------
	// -------------------------------------------------------------

	Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4::Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
	Vector4::Vector4(Vector3 vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}

	Vector4 Vector4::operator-(const Vector4& vec) const
	{
		return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	Vector4 Vector4::operator+(const Vector4& vec) const
	{
		return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

	bool Vector4::Equals(const Vector4& vec1, const Vector4& vec2, float epsilon)
	{
		return std::abs(vec1.x - vec2.x) <= epsilon &&
			std::abs(vec1.y - vec2.y) <= epsilon &&
			std::abs(vec1.z - vec2.z) <= epsilon &&
			std::abs(vec1.w - vec2.w) <= epsilon;
	}

	Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2, const float wVal)
	{
		return Vector4
		(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x,
			wVal
		);
	}

// 	Vector3d::Vector3d(const Vector4& v)
// 	{
// 		x = static_cast<double>(v.x);
// 		y = static_cast<double>(v.y);
// 		z = static_cast<double>(v.z);
// 	}
}

