#include "pch.h"
#include "Quaternion.h"

namespace BMath
{
	Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}
	Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
	Quaternion::Quaternion(const float angle, const BMath::Vector3& axis) : w(1), x(0), y(0), z(0)
	{
		SetFromAxisAngle(angle, axis);
	}

	Quaternion& Quaternion::Normalize()
	{
		float n = Length();
		if (n > 0)
		{
			float invNorm = 1.0f / n;
			w *= invNorm;
			x *= invNorm;
			y *= invNorm;
			z *= invNorm;
		}
		return *this;
	}

	Quaternion Quaternion::Normalize(const Quaternion& q)
	{
		Quaternion resultQ{};
		float n = q.Length();
		if (n > 0)
		{
			float invNorm = 1.0f / n;
			resultQ.w = q.w * invNorm;
			resultQ.x = q.x * invNorm;
			resultQ.y = q.y * invNorm;
			resultQ.z = q.z * invNorm;
		}
		return resultQ;
	}

	bool Quaternion::Equals(const Quaternion& q, const float e) const
	{
		return  std::abs(x - q.x) <= e &&
				std::abs(w - q.w) <= e &&
				std::abs(y - q.y) <= e &&
				std::abs(z - q.z) <= e;
	}

	bool Quaternion::Equals(const Quaternion& q1, const Quaternion& q2, float e)
	{
		return  std::abs(q1.x - q2.x) <= e &&
				std::abs(q1.w - q2.w) <= e &&
				std::abs(q1.y - q2.y) <= e &&
				std::abs(q1.z - q2.z) <= e;
	}

	Quaternion& Quaternion::Conjugate()
	{ 
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	// Multiplication with another quaternion
	Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		return Quaternion(
			w * q.w - x * q.x - y * q.y - z * q.z,
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w
		);
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q)
	{
		float newW = w * q.w - x * q.x - y * q.y - z * q.z;
		float newX = w * q.x + x * q.w + y * q.z - z * q.y;
		float newY = w * q.y - x * q.z + y * q.w + z * q.x;
		float newZ = w * q.z + x * q.y - y * q.x + z * q.w;

		w = newW;
		x = newX;
		y = newY;
		z = newZ;

		return *this;
	}

	bool Quaternion::operator==(const Quaternion& q) const
	{
		return w == q.w && x == q.x && y == q.y && z == q.z;
	}

	Quaternion& Quaternion::operator= (const Quaternion& q)
	{
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
		return *this;
	}


	// Directly set the quaternion from an axis-angle representation (axis must be normalized)
	Quaternion& Quaternion::SetFromAxisAngle(const float angle, const BMath::Vector3& axis)
	{
		BMath::Vector3 normalizedAxis = BMath::Vector3::Normalize(axis);
		float halfAngle = angle / 2;
		float sinHalfAngle = sin(halfAngle);
		w = cos(halfAngle);
		x = normalizedAxis.x * sinHalfAngle;
		y = normalizedAxis.y * sinHalfAngle;
		z = normalizedAxis.z * sinHalfAngle;
		Normalize(); // Ensure it's a unit quaternion
		return *this;
	}

	Matrix4x4f Quaternion::QuaternionToMatrix(const Quaternion& q) 
	{
		float xx = q.x * q.x;
		float yy = q.y * q.y;
		float zz = q.z * q.z;
		float xy = q.x * q.y;
		float xz = q.x * q.z;
		float yz = q.y * q.z;
		float wx = q.w * q.x;
		float wy = q.w * q.y;
		float wz = q.w * q.z;

		Matrix4x4f rotMatrix;
		rotMatrix._11 = 1 - 2 * (yy + zz);
		rotMatrix._12 = 2 * (xy - wz);
		rotMatrix._13 = 2 * (xz + wy);
		rotMatrix._14 = 0;

		rotMatrix._21 = 2 * (xy + wz);
		rotMatrix._22 = 1 - 2 * (xx + zz);
		rotMatrix._23 = 2 * (yz - wx);
		rotMatrix._24 = 0;

		rotMatrix._31 = 2 * (xz - wy);
		rotMatrix._32 = 2 * (yz + wx);
		rotMatrix._33 = 1 - 2 * (xx + yy);
		rotMatrix._34 = 0;

		rotMatrix._41 = 0;
		rotMatrix._42 = 0;
		rotMatrix._43 = 0;
		rotMatrix._44 = 1;

		return rotMatrix;
	}
}