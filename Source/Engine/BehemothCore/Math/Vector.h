#pragma once

#include "Matrix.h"

namespace Math
{
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
		Vector3() = default;
		Vector3(const float x, const float y, const float z);

		float x;
		float y;
		float z;

		static Vector3 Forward();
		static Vector3 Right();
		static Vector3 Up();

		Vector3& operator= (const Vector3& vec);
		Vector3  operator* (const Vector3& vec) const;
		Vector3  operator- (const Vector3& vec)const ;

		static float Angle(const Vector3& v1, const Vector3& v2);
		static float SquaredMagnitude(const Vector3& v1);
		static float Magnitude(const Vector3& v1);
		static float Distance(const Vector3& v1, const Vector3& v2);
		static float Dot(const Vector3& v1, const Vector3& v2);
		static Vector3 Normalize(const Vector3& v1);
		static Vector3 Cross(const Vector3& v1, const Vector3& v2);

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

		Vector4 operator* (const float val) const;
		Vector4 operator* (const Matrix4x4& m) const;
		Vector4 operator/ (const float val) const;

	private:

	};
}

