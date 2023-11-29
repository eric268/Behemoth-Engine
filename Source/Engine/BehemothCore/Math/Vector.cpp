#include "Vector.h"

namespace Math
{

	Vector2::Vector2(const float x, const float y) : x(x), y(y) {}

	Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

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

	Vector4::Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
}

