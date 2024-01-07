#pragma once
#include "Vector.h"
#include "Matrix.h"

namespace BMath
{
	class Quaternion
	{
	public:
		// Constructors
		Quaternion();
		Quaternion(float w, float x, float y, float z);
		Quaternion(float angle, BMath::Vector3 axis);

		// inline functions
		inline float Length() const { return sqrt(w * w + x * x + y * y + z * z); }
		inline Quaternion Rotate(Quaternion& point) const { return (*this) * point * Conjugate(); }
		inline Quaternion Inverse() const { return Conjugate().Normalize(); }
		inline Quaternion Conjugate() const { return Quaternion(w, -x, -y, -z);}

		static Matrix4x4 QuaternionToMatrix(const Quaternion& q);
		static Quaternion Normalize(const Quaternion& q);
		Quaternion& Normalize();
		Quaternion operator*(const Quaternion& q) const;
		Quaternion& operator*=(const Quaternion& q);

		Quaternion& SetFromAxisAngle(const float& angle, BMath::Vector3 axis);

		union 
		{
			float data[4];
			struct 
			{
				float x;
				float y;
				float z;
				float w;
			};
		};

	private:
	};
}

