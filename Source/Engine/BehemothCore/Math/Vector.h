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
		static Vector3 Reflect(const Vector3& lightDir, const Vector3& normal);

		static BMath::Vector3 VectorProject(const BMath::Vector3& a, const BMath::Vector3& b) 
		{
			float dotProduct = Vector3::Dot(a,b); // Assuming there's a Dot method for Vector3
			float magnitudeSquared = Vector3::Dot(b,b);
			if (magnitudeSquared == 0.0f) 
			{
				// Avoid division by zero
				return BMath::Vector3(0.0f, 0.0f, 0.0f);
			}
			return b * (dotProduct / magnitudeSquared);
		}

		inline std::string Print()
		{
			return std::string(" X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z));
		}

		inline float Magnitude() const
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
			const float x = v1.x - v2.x;
			const float y = v1.y - v2.y;
			const float z = v1.z - v2.z;
			return std::sqrt((x * x) + (y * y) + (z * z));
		}

		template <FloatOrDouble T = float>
		static T SquaredDistance(const Vector3& v1, const Vector3& v2)
		{
			const float x = v1.x - v2.x;
			const float y = v1.y - v2.y;
			const float z = v1.z - v2.z;
			return (x * x) + (y * y) + (z * z);
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

		inline std::string Print()
		{
			return std::string(" X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z) + " W: " + std::to_string(w));
		}
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

