#pragma once

#include "Matrix.h"

namespace Math
{
	class Vector2;
	class Vector3;
	class Vector4;

	class Vector2
	{
	public:
		Vector2() = default;
		Vector2(const float x, const float y);
		
		float x;
		float y;

	private:

	};


	class Vector3
	{
	public:
		Vector3();
		Vector3(const float x, const float y, const float z);
		explicit Vector3(Vector4 vec);

		float x;
		float y;
		float z;

		static Vector3 Forward();
		static Vector3 Right();
		static Vector3 Up();
		static Vector3 Zero();

		Vector3& operator= (const Vector3& vec);
		Vector3  operator* (const Vector3& vec) const;
		Vector3  operator- (const Vector3& vec)const ;
		Vector3  operator*= (const Vector3 vec) const;

		template<typename T>
		Vector3 operator* (const T scalar) const
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}
		template<typename T>
		Vector3 operator*= (const T scalar) const
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}

		template<typename T>
		Vector3 operator* (const T scalar)
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}
		template<typename T>
		Vector3 operator*= (const T scalar)
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}

		static float Angle(const Vector3& v1, const Vector3& v2);
		static float SquaredMagnitude(const Vector3& v1);
		static float Magnitude(const Vector3& v1);
		static float Distance(const Vector3& v1, const Vector3& v2);
		static float Dot(const Vector3& v1, const Vector3& v2);
		static Vector3 Normalize(const Vector3& v1);
		static Vector3 Cross(const Vector3& v1, const Vector3& v2);
		static bool Equals(const Vector3& v1, const Vector3& v2, const float epsilon = 1e-2);

		static Vector3& RotateVector(Vector3& vec, const Matrix4x4& rotationMatrix);

	private:

	};


	class Vector4
	{
	public:
		Vector4();
		Vector4(const float x, const float y, const float z, const float w);
		Vector4(Vector3 vec, float w);

		float x;
		float y;
		float z;
		float w;

		static bool Equals(const Vector4& vec1, const Vector4& vec2, float epsilon = 1e-2);
		static Vector4 Cross(const Vector4& v1, const Vector4& v2, const float wVal = 1.0f);

		Vector4 operator* (const Matrix4x4& m) const;

		template<typename T>
		Vector4 operator* (const T scalar) const
		{
			return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		template<typename T>
		Vector4 operator*=(const T scalar) const
		{
			return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		template<typename T>
		Vector4 operator/ (const T scalar) const
		{
			assert(scalar != 0);
			return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
		}

		Vector4 operator-(const Vector4 vec) const;

	private:

	};
}

