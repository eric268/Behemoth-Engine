#pragma once

#include <cassert>


namespace BMath
{
	template<typename T>
	concept FloatOrDouble = std::is_same<T, float>::value || std::is_same<T, double>::value;

	template<FloatOrDouble T>
	struct BVector3;

	template<FloatOrDouble T>
	struct BVector4;

	template <FloatOrDouble T>
	struct BVector2
	{
		union
		{
			T data[2];
			struct
			{
				T x;
				T y;
			};
		};

		BVector2() = default;
		BVector2(const T x, const T y) : x(x), y(y) {}
	};

	using Vector2 = BVector2<float>;
	using Vector2d = BVector2<double>;

	template <FloatOrDouble T>
	struct BVector3
	{
	public:
		union
		{
			T data[3];
			struct 
			{
				T x;
				T y;
				T z;
			};
		};

		BVector3() : x(0.0f), y(0.0f), z(0.0f) {}

		BVector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

		explicit BVector3(BVector4<T> vec) : x(vec.x), y(vec.y), z(vec.z) {}

		explicit BVector3(const float val) : x(val), y(val), z(val) {}

		BVector3& Normalize()
		{
			float mag = Magnitude();

			if (mag == 0)
			{
				*this = BVector3::Zero();
				return *this;
			}

			x /= mag;
			y /= mag;
			z /= mag;

			return *this;
		}

		static bool Equals(const BVector3& v1, const BVector3& v2, const T epsilon = 1e-2)
		{
			return  std::abs(v1.x - v2.x) <= epsilon &&
				std::abs(v1.y - v2.y) <= epsilon &&
				std::abs(v1.z - v2.z) <= epsilon;
		}

		inline std::string Print()
		{
			return std::string(" X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z));
		}

		inline float Magnitude() const
		{
			return std:: sqrt(x * x + y * y + z * z);
		}

		static T Dot(const BVector3& v1, const BVector3& v2)
		{
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}

		static BVector3 Forward()
		{
			BVector3 vec{};
			vec.z = -1.0f;
			return vec;
		}
		static BVector3 Right()
		{
			BVector3 vec{};
			vec.x = 1.0f;
			return vec;
		}
		static BVector3 Up()
		{
			BVector3 vec;
			vec.y = 1.0f;
			return vec;
		}

		static BVector3 Zero()
		{
			return BVector3{};
		}

		static BVector3 One()
		{
			return BVector3(1.0f, 1.0f, 1.0f);
		}

		static BVector3 Reflect(const BVector3& lightDir, const BVector3& normal)
		{
			return BVector3<T>::Normalize(lightDir - (normal * BVector3<T>::Dot(normal, lightDir) * (T)2.0));
		}

		static BVector3 VectorProject(const BVector3& a, const BVector3& b)
		{
			T dotProduct = BVector3<T>::Dot(a, b);
			T magnitudeSquared = BVector3<T>::Dot(b, b);
			if (magnitudeSquared == 0.0f)
			{
				// Avoid division by zero
				return BVector3(0.0f, 0.0f, 0.0f);
			}
			return b * (dotProduct / magnitudeSquared);
		}

		static T Angle(const BVector3& v1, const BVector3& v2)
		{
			T dot = BVector3<T>::Dot(v1, v2);
			T magnitudeProduct = BVector3<T>::Magnitude(v1) * BVector3<T>::Magnitude(v2);
			T val = std::max(-1.0f, std::min(1.0f, dot / magnitudeProduct));
			return std::acos(val);
		}

		static BVector3 Normalize(const BVector3& vec)
		{
			const T mag = BVector3<T>::Magnitude(vec);

			if (mag == 0)
			{
				return BVector3<T>::Zero();
			}

			return BVector3(vec.x / mag, vec.y / mag, vec.z / mag);
		}


		static BVector3 Cross(const BVector3& v1, const BVector3& v2)
		{
			return BVector3
			(
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x
			);
		}

		static T Magnitude(const BVector3& v1)
		{
			return std::sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
		}

		static T SquaredMagnitude(const BVector3& v1)
		{
			return (v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
		}

		static T Distance(const BVector3& v1, const BVector3& v2)
		{
			const T x = v1.x - v2.x;
			const T y = v1.y - v2.y;
			const T z = v1.z - v2.z;
			return std::sqrt((x * x) + (y * y) + (z * z));
		}

		static T SquaredDistance(const BVector3& v1, const BVector3& v2)
		{
			const T x = v1.x - v2.x;
			const T y = v1.y - v2.y;
			const T z = v1.z - v2.z;
			return (x * x) + (y * y) + (z * z);
		}

#pragma region Overloaded Operators

		T& operator[](int i)
		{
			return data[i];
		}

		const T& operator[](int i) const
		{
			return data[i];
		}

		bool operator == (const BVector3& vec) const
		{
			return this->x == vec.x && this->y == vec.y && this->z == vec.z;
		}

		bool operator != (const BVector3& vec) const
		{
			return !(*this == vec);
		}

		BVector3 operator-() const
		{
			return BVector3(-x, -y, -z);
		}

		template<typename U>
		BVector3 operator* (const U scalar) const
		{
			return BVector3(x * scalar, y * scalar, z * scalar);
		}

		template<typename U>
		BVector3 operator*= (U scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		template<typename U>
		BVector3 operator+(const U val) const
		{
			return BVector3(x + val, y + val, z + val);
		}

		template<typename U>
		BVector3& operator+=(const U scalar)
		{
			x += scalar;
			y += scalar;
			z += scalar;
			return *this;
		}

		template<typename U>
		BVector3 operator-(const U val) const
		{
			return BVector3(x - val, y - val, z - val);
		}

		template<typename U>
		BVector3& operator-=(const U val)
		{
			x -= val;
			y -= val;
			z -= val;
			return *this;
		}

		template<typename U>
		BVector3 operator/(const U val) const
		{
			assert(val != 0.0f);
			return BVector3(x / val, y / val, z / val);
		}

		template<typename U>
		BVector3& operator/=(const U val)
		{
			assert(val != 0.0f);
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}

		BVector3 operator+(const BVector3& vec) const
		{
			return BVector3(x + vec.x, y + vec.y, z + vec.z);
		}

		BVector3& operator+=(const BVector3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}

		BVector3 operator*(const BVector3& vec) const
		{
			return BVector3(x * vec.x, y * vec.y, z * vec.z);
		}

		BVector3& operator*=(const BVector3& vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			return *this;
		}

		BVector3 operator-(const BVector3 vec) const
		{
			return BVector3(x - vec.x, y - vec.y, z - vec.z);
		}

		BVector3& operator-=(const BVector3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}

		BVector3 operator/(const BVector3& vec) const
		{
			assert(vec.x != 0.0f && vec.y != 0.0f && vec.z != 0.0f);
			return Vector3(x / vec.x, y / vec.y, z / vec.z);
		}

		BVector3& operator/=(const BVector3& vec)
		{
			assert(vec.x != 0.0f && vec.y != 0.0f && vec.z != 0.0f);
			x /= vec.x;
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			return *this;
		}
#pragma endregion 
	};

	using Vector3 = BVector3<float>;
	using Vector3d = BVector3<double>;

	template<FloatOrDouble T>
	struct BVector4
	{
		BVector4() : x((T)0.0), y((T)0.0f), z((T)0.0f), w((T)0.0f) {}
		BVector4(const T x, const T y, const T z, const T w) : x(x), y(y), z(z), w(w) {}
		BVector4(BVector3<T> vec, T w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}

		union
		{
			float data[4];
			struct
			{
				T x;
				T y;
				T z;
				T w;
			};
		};

		static bool Equals(const BVector4& vec1, const BVector4& vec2, T epsilon = 1e-2)
		{
			return std::abs(vec1.x - vec2.x) <= epsilon &&
				std::abs(vec1.y - vec2.y) <= epsilon &&
				std::abs(vec1.z - vec2.z) <= epsilon &&
				std::abs(vec1.w - vec2.w) <= epsilon;
		}

		static BVector4 Cross(const BVector4& v1, const BVector4& v2, const T wVal = (T)0.0)
		{
			return BVector4
			(
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x,
				wVal
			);
		}

		inline std::string Print()
		{
			return std::string(" X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z) + " W: " + std::to_string(w));
		}

		T& operator[](int i)
		{
			return data[i];
		}

		const T& operator[](int i) const
		{
			return data[i];
		}

		template<typename U>
		BVector4 operator* (const U scalar) const
		{
			return BVector4(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		template<typename U>
		BVector4& operator*=(const U scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		template<typename U>
		BVector4 operator/ (const U scalar) const
		{
			assert(scalar != 0);
			return BVector4(x / scalar, y / scalar, z / scalar, w / scalar);
		}

		BVector4 operator-(const BVector4& vec) const
		{
			return BVector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}

		BVector4 operator+(const BVector4& vec) const
		{
			return BVector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}
	};

	using Vector4 = BVector4<float>;
	using Vector4d = BVector4<double>;
}

