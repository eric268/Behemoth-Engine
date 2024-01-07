#include "pch.h"
#include "Quaternion.h"

namespace BMath
{
	Quaternion::Quaternion() : x(0), y(0), z(0), w(1) {}
	Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
	Quaternion::Quaternion(float angle, BMath::Vector3 axis) : x(0), y(0), z(0), w(1)
	{
		*this = SetFromAxisAngle(angle, axis);
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

	// Multiplication with another quaternion
	Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		return Quaternion(
			w * q.w - x * q.x - y * q.y - z * q.z, // new w
			w * q.x + x * q.w + y * q.z - z * q.y, // new x
			w * q.y - x * q.z + y * q.w + z * q.x, // new y
			w * q.z + x * q.y - y * q.x + z * q.w  // new z
		);
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q)
	{
		this->w = this->w * q.w - x * q.x - y * q.y - z * q.z; // new w
		this->x = this->w * q.x + x * q.w + y * q.z - z * q.y; // new x
		this->y = this->w * q.y - x * q.z + y * q.w + z * q.x; // new y
		this->z = this->w * q.z + x * q.y - y * q.x + z * q.w; // new z	
		return *this;
	}



	// Directly set the quaternion from an axis-angle representation (axis must be normalized)
	Quaternion& Quaternion::SetFromAxisAngle(const float& angle, BMath::Vector3 axis)
	{
		float halfAngle = angle / 2;
		float sinHalfAngle = sin(halfAngle);
		w = cos(halfAngle);
		x = axis.x * sinHalfAngle;
		y = axis.y * sinHalfAngle;
		z = axis.z * sinHalfAngle;
		Normalize(); // Ensure it's a unit quaternion
		return *this;
	}

	Matrix4x4 Quaternion::QuaternionToMatrix(const Quaternion& q) 
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

		Matrix4x4 rotMatrix;
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