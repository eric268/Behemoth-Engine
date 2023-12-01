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

	Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

	Vector4::Vector4(Vector3 vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}

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

	Vector3& Vector3::operator= (const Vector3& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		return *this;
	}

	Vector3 Vector3::operator* (const Vector3& vec) const
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



	// -------------------------------------------------------------
	// -------------------------------------------------------------
	// --------------------   Vector 4   ---------------------------
	// -------------------------------------------------------------
	// -------------------------------------------------------------

	Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4::Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

	Vector4 Vector4::operator* (const float val) const
	{
		Vector4 v;
		v.x = x * val;
		v.y = y * val;
		v.z = z * val;
		v.w = w * val;
		return v;
	}

	Vector4 Vector4::operator* (const Matrix4x4& m) const
	{
		Vector4 result;

		result.x = x * m.data[0][0] + y * m.data[1][0] + z * m.data[2][0] + w * m.data[3][0];
		result.y = x * m.data[0][1] + y * m.data[1][1] + z * m.data[2][1] + w * m.data[3][1];
		result.z = x * m.data[0][2] + y * m.data[1][2] + z * m.data[2][2] + w * m.data[3][2];
		result.w = x * m.data[0][3] + y * m.data[1][3] + z * m.data[2][3] + w * m.data[3][3];

		return result;
	}

	Vector4 Vector4::operator/ (const float val) const
	{
		Vector4 vec;

		vec.x = x / val;
		vec.y = y / val;
		vec.z = z / val;
		vec.w = w / val;

		return vec;
	}

	bool Vector4::Equals(const Vector4& vec1, const Vector4& vec2, float epsilon)
	{
		return std::abs(vec1.x - vec2.x) <= epsilon &&
			   std::abs(vec1.y - vec2.y) <= epsilon &&
			   std::abs(vec1.z - vec2.z) <= epsilon &&
			   std::abs(vec1.w - vec2.w) <= epsilon;
	}
}

