#pragma once
#include "MathCore.h"

namespace BMath
{
	class Quaternion
	{
	public:
		// Constructors
		Quaternion();
		Quaternion(float w, float x, float y, float z);
		Quaternion(const float angle, const BMath::Vector3& axis);

		// inline functions
		inline float Length() const { return sqrt(x * x + y * y + z * z + w * w); }
		inline bool operator!=(const Quaternion& q) const { return *this != q; }
		inline Quaternion& Inverse() { return Conjugate().Normalize(); }

		Quaternion Rotate(Quaternion& point) { return (*this) * point * Conjugate(); }
		Quaternion& Conjugate();

		static Quaternion Identity() { return Quaternion(); }
		static BMatrix4x4 QuaternionToMatrix(const Quaternion& q);
		static Quaternion Normalize(const Quaternion& q);
		bool Equals(const Quaternion& q, float e = EPSILON) const;
		static bool Equals(const Quaternion& q1, const Quaternion& q2, float e = 1e-2);
		Quaternion& Normalize();
		Quaternion operator*(const Quaternion& q) const;
		Quaternion& operator*=(const Quaternion& q);
		bool operator==(const Quaternion& q) const;
		Quaternion& operator= (const Quaternion& q);

		Quaternion& SetFromAxisAngle(const float angle, const BMath::Vector3& axis);

		union
		{
			float data[4];
			struct
			{
				float w;
				float x;
				float y;
				float z;
			};
		};

	private:
	};
}

