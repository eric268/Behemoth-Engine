#pragma once

#include <cassert>

namespace Math
{
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix4x4;

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
		Vector3(const float val);
		explicit Vector3(Vector4 vec);

		union
		{
			float data[3];
			struct 
			{
				float x;
				float y;
				float z;
			};

		};

		Vector3& Normalize();
		float Magnitude() const;

		static Vector3 Forward();
		static Vector3 Right();
		static Vector3 Up();
		static Vector3 Zero();
		static Vector3 One();

		static Vector3 Normalize(const Vector3& v1);
		static Vector3 Cross(const Vector3& v1, const Vector3& v2);
		static Vector3& RotateVector(Vector3& vec, const Matrix4x4& rotationMatrix, float w = 1.0f);
		static Vector3 Reflect(const Vector3& lightDir, const Vector3& normal);

		static float Angle(const Vector3& v1, const Vector3& v2);
		static float SquaredMagnitude(const Vector3& v1);
		static float Magnitude(const Vector3& v1);
		static float Distance(const Vector3& v1, const Vector3& v2);
		static float SquaredDistance(const Vector3& v1, const Vector3& v2);
		static float Dot(const Vector3& v1, const Vector3& v2);

		static bool Equals(const Vector3& v1, const Vector3& v2, const float epsilon = 1e-2);

#pragma region Overloaded Operators

		float& operator[](int i)
		{
			return data[i];
		}

		const float& operator[](int i) const
		{
			return data[i];
		}

		Vector3 operator-() const
		{
			return Vector3(-x, -y, -z);
		}

		template<typename T>
		Vector3 operator* (const T scalar) const
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}

		template<typename T>
		Vector3 operator*= (T scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		template<typename T>
		Vector3 operator+(const T val) const 
		{
			return Vector3(x + val, y + val, z + val);
		}

		template<typename T>
		Vector3& operator+=(const T scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			return *this;
		}

		template<typename T>
		Vector3 operator-(const T val) const
		{
			return Vector3(x - val, y - val, z - val);
		}

		template<typename T>
		Vector3& operator-=(const T val)
		{
			x -= val;
			y -= val;
			z -= val;
			return *this;
		}

		template<typename T>
		Vector3 operator/(const T val) const
		{
			assert(val != 0.0f);
			return Vector3(x / val, y / val, z / val);
		}

		template<typename T>
		Vector3& operator/=(const T val)
		{
			assert(val != 0.0f);
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}

		Vector3 operator+(const Vector3& vec) const
		{
			return Vector3(x + vec.x, y + vec.y, z + vec.z);
		}

		Vector3& operator+=(const Vector3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}

		Vector3 operator*(const Vector3& vec) const
		{
			return Vector3(x * vec.x, y * vec.y, z * vec.z);
		}

		Vector3& operator*=(const Vector3& vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			return *this;
		}

		Vector3 operator-(const Vector3 vec) const
		{
			return Vector3(x - vec.x, y - vec.y, z - vec.z);
		}

		Vector3& operator-=(const Vector3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}

		Vector3 operator/(const Vector3& vec) const
		{
			assert(vec.x != 0.0f && vec.y != 0.0f && vec.z != 0.0f);
			return Vector3(x / vec.x, y / vec.y, z / vec.z);
		}

		Vector3& operator/=(const Vector3& vec)
		{
			assert(vec.x != 0.0f && vec.y != 0.0f && vec.z != 0.0f);
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			return *this;
		}
#pragma endregion 
	private:
	};

	class Vector4
	{
	public:
		Vector4();
		Vector4(const float x, const float y, const float z, const float w);
		Vector4(Vector3 vec, float w);

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


		static bool Equals(const Vector4& vec1, const Vector4& vec2, float epsilon = 1e-2);
		static Vector4 Cross(const Vector4& v1, const Vector4& v2, const float wVal = 1.0f);

		float& operator[](int i)
		{
			return data[i];
		}

		const float& operator[](int i) const
		{
			return data[i];
		}

		template<typename T>
		Vector4 operator* (const T scalar) const
		{
			return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		template<typename T>
		Vector4& operator*=(const T scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
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

