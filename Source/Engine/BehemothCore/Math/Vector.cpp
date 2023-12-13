#include "MathCore.h"


namespace Math
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

	Vector3 Vector3::Forward()
	{
		Vector3 vec;
		vec.x = 1.0f;
		return vec;
	}
	Vector3 Vector3::Right()
	{
		Vector3 vec;
		vec.y = 1.0f;
		return vec;
	}
	Vector3 Vector3::Up()
	{
		Vector3 vec;
		vec.z = 1.0f;
		return vec;
	}

	Vector3 Vector3::Zero()
	{
		return Vector3{};
	}

	Vector3& Vector3::operator= (const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	Vector3 Vector3::operator* (const Vector3& vec) const
	{
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}

	Vector3 Vector3::operator*= (const Vector3 vec) const
	{
		Vector3 v;
		v.x = x * vec.x;
		v.y = y * vec.y;
		v.z = z * vec.z;
		return v;
	}

	Vector3 Vector3::operator- (const Vector3& vec) const
	{
		Vector3 v;
		v.x = x - vec.x;
		v.y = y - vec.y;
		v.z = z - vec.z;
		return v;
	}

	float Vector3::Magnitude() const
	{
		return sqrt(x * x + y * y + z * z);
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

	float Vector3::Angle(const Vector3& v1, const Vector3& v2)
	{
		float dot = Dot(v1, v2);
		float magnitudeProduct = Magnitude(v1) * Magnitude(v2);
		float val = std::max(-1.0f, std::min(1.0f, dot / magnitudeProduct));
		return std::acos(val);
	}

	float Vector3::Magnitude(const Vector3& v1)
	{
		return std::sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	}

	float Vector3::SquaredMagnitude(const Vector3& v1)
	{
		return (v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	}

	float Vector3::Distance(const Vector3& v1, const Vector3& v2)
	{
		const Vector3 v = v1 - v2;
		return Magnitude(v1 - v2);
	}


	Vector3 Vector3::Normalize(const Vector3& vec)
	{
		const float mag = Magnitude(vec);

		if (mag == 0)
			return Vector3::Zero();

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


	float Vector3::Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	bool Vector3::Equals(const Vector3& v1, const Vector3& v2, const float epsilon)
	{
		return  std::abs(v1.x - v2.x) <= epsilon &&
			std::abs(v1.y - v2.y) <= epsilon &&
			std::abs(v1.z - v2.z) <= epsilon;
	}

	Vector3& Vector3::RotateVector(Vector3& vec, const Matrix4x4& rotationMatrix)
	{
		Vector4 v = rotationMatrix * Vector4(vec, 0.0f);

		vec.x = v.x;
		vec.y = v.y;
		vec.z = v.z;

		Vector3::Normalize(vec);
		return vec;
	}


	// -------------------------------------------------------------
	// -------------------------------------------------------------
	// --------------------   Vector 4   ---------------------------
	// -------------------------------------------------------------
	// -------------------------------------------------------------

	Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4::Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
	Vector4::Vector4(Vector3 vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}

	Vector4 Vector4::operator-(const Vector4 vec) const
	{
		return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
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
}

