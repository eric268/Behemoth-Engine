#pragma once

#include <cassert>


namespace BMath
{
	class Vector2;
	class Vector3;
	class Vector3d;
	class Vector4;

	template<typename T>
	concept FloatOrDouble = std::is_same<T, float>::value || std::is_same<T, double>::value;

	template<FloatOrDouble T>
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
		explicit Vector3(const float val);
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

		static Vector3 Forward();
		static Vector3 Right();
		static Vector3 Up();
		static Vector3 Zero();
		static Vector3 One();

		static Vector3 Normalize(const Vector3& v1);
		static Vector3 Cross(const Vector3& v1, const Vector3& v2);
		static Vector3& RotateVector(Vector3& vec, const Matrix4x4<float>& rotationMatrix, float w = 1.0f);
		static Vector3 Reflect(const Vector3& lightDir, const Vector3& normal);

		std::string Print()
		{
			return std::string(" X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z));
		}

		float Magnitude() const
		{
			return std:: sqrt(x * x + y * y + z * z);
		}

		template <FloatOrDouble T = float>
		static T Dot(const Vector3& v1, const Vector3& v2)
		{
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}

		template <FloatOrDouble T = float>
		static T Angle(const Vector3& v1, const Vector3& v2)
		{
			T dot = Dot<T>(v1, v2);
			T magnitudeProduct = Magnitude(v1) * Magnitude(v2);
			T val = std::max(-1.0f, std::min(1.0f, dot / magnitudeProduct));
			return std::acos(val);
		}

		template <FloatOrDouble T = float>
		static T Magnitude(const Vector3& v1)
		{
			return std::sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
		}

		template <FloatOrDouble T = float>
		static float SquaredMagnitude(const Vector3& v1)
		{
			return (v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
		}

		template <FloatOrDouble T = float>
		static T Distance(const Vector3& v1, const Vector3& v2)
		{
			const Vector3 v = v1 - v2;
			return Magnitude(v1 - v2);
		}

		template <FloatOrDouble T = float>
		static T SquaredDistance(const Vector3& v1, const Vector3& v2)
		{
			const Vector3 v = v1 - v2;
			return SquaredMagnitude(v1 - v2);
		}

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

		bool operator == (const Vector3& vec) const
		{
			return this->x == vec.x && this->y == vec.y && this->z == vec.z;
		}

		bool operator != (const Vector3& vec) const
		{
			return !(*this == vec);
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
// 
// 	struct Vector3d : public Vector3
// 	{
// 	public:
// 
// 		Vector3d(float x, float y, float z) : x(x), y(y), z(z) {}
// 		Vector3d() : x(0.0), y(0.0), z(0.0) {}
// 		Vector3d(Vector3 v)
// 		{
// 			x = static_cast<double>(v.x);
// 			y = static_cast<double>(v.y);
// 			z = static_cast<double>(v.z);
// 		}
// 
// 	/*	 Vector3d(const Vector4& v);*/
// 
// 		union
// 		{
// 			double data[3];
// 			struct
// 			{
// 				double x;
// 				double y;
// 				double z;
// 			};
// 
// 		};
// 	};

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

		Vector4 operator-(const Vector4& vec) const;
		Vector4 operator+(const Vector4& vec) const;

	private:

	};
}

