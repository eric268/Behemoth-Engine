#pragma once
#include "MathCore.h"

namespace BMath
{
	template <typename T>
	struct BQuaternion
	{
		// Constructors
		BQuaternion() : w((T)1), x((T)0), y((T)0), z((T)0) {}
		BQuaternion(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}
		BQuaternion(const T angle, const BMath::Vector3& axis) : w(1), x(0), y(0), z(0)
		{
			SetFromAxisAngle(angle, axis);
		}

		// inline functions
		inline T Length() const { return sqrt(x * x + y * y + z * z + w * w); }
		inline bool operator!=(const BQuaternion<T>& q) const { return !(*this == q); }
		inline BQuaternion<T>& Inverse() { return Conjugate().Normalize(); }
		BQuaternion<T> Rotate(BQuaternion<T>& point) { return (*this) * point * Conjugate(); }
		static BQuaternion<T> Identity() { return BQuaternion<T>((T)1.0, (T)0.0, (T)0.0, (T)0.0); }


		BQuaternion<T>& Normalize()
		{
			T n = Length();
			if (n > 0)
			{
				T invNorm = 1.0f / n;
				w *= invNorm;
				x *= invNorm;
				y *= invNorm;
				z *= invNorm;
			}
			return *this;
		}

		static BQuaternion<T> Normalize(const BQuaternion<T>& q)
		{
			BQuaternion<T> resultQ{};
			T n = q.Length();
			if (n > 0)
			{
				T invNorm = 1.0f / n;
				resultQ.w = q.w * invNorm;
				resultQ.x = q.x * invNorm;
				resultQ.y = q.y * invNorm;
				resultQ.z = q.z * invNorm;
			}
			return resultQ;
		}

		bool Equals(const BQuaternion<T>& q, const T e = 1e-2) const
		{
			return  std::abs(x - q.x) <= e &&
				std::abs(w - q.w) <= e &&
				std::abs(y - q.y) <= e &&
				std::abs(z - q.z) <= e;
		}

		static bool Equals(const BQuaternion<T>& q1, const BQuaternion<T>& q2, T e = 1e-2)
		{
			return  std::abs(q1.x - q2.x) <= e &&
				std::abs(q1.w - q2.w) <= e &&
				std::abs(q1.y - q2.y) <= e &&
				std::abs(q1.z - q2.z) <= e;
		}

		BQuaternion<T>& Conjugate()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

		// Multiplication with another quaternion
		BQuaternion<T> operator*(const BQuaternion<T>& q) const
		{
			return BQuaternion<T>(
				w * q.w - x * q.x - y * q.y - z * q.z,
				w * q.x + x * q.w + y * q.z - z * q.y,
				w * q.y - x * q.z + y * q.w + z * q.x,
				w * q.z + x * q.y - y * q.x + z * q.w
			);
		}

		BQuaternion<T>& operator*=(const BQuaternion<T>& q)
		{
			T newW = w * q.w - x * q.x - y * q.y - z * q.z;
			T newX = w * q.x + x * q.w + y * q.z - z * q.y;
			T newY = w * q.y - x * q.z + y * q.w + z * q.x;
			T newZ = w * q.z + x * q.y - y * q.x + z * q.w;

			w = newW;
			x = newX;
			y = newY;
			z = newZ;

			return *this;
		}

		bool operator==(const BQuaternion<T>& q) const
		{
			return w == q.w && x == q.x && y == q.y && z == q.z;
		}

		BQuaternion<T>& operator= (const BQuaternion<T>& q)
		{
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
			return *this;
		}

		// Directly set the quaternion from an axis-angle representation (axis must be normalized)
		BQuaternion<T>& SetFromAxisAngle(const T angle, const BMath::Vector3& axis)
		{
			BMath::Vector3 normalizedAxis = BMath::Vector3::Normalize(axis);
			T halfAngle = angle / 2;
			T sinHalfAngle = sin(halfAngle);
			w = cos(halfAngle);
			x = normalizedAxis.x * sinHalfAngle;
			y = normalizedAxis.y * sinHalfAngle;
			z = normalizedAxis.z * sinHalfAngle;
			Normalize(); // Ensure it's a unit quaternion
			return *this;
		}

		static BMatrix4x4<T> QuaternionToMatrix(const BQuaternion<T>& q)
		{
			T xx = q.x * q.x;
			T yy = q.y * q.y;
			T zz = q.z * q.z;
			T xy = q.x * q.y;
			T xz = q.x * q.z;
			T yz = q.y * q.z;
			T wx = q.w * q.x;
			T wy = q.w * q.y;
			T wz = q.w * q.z;

			BMatrix4x4<T> rotMatrix;
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

		union
		{
			T data[4];
			struct
			{
				T w;
				T x;
				T y;
				T z;
			};
		};
	};
	using Quaternion = BQuaternion<float>;
	using Quaterniond = BQuaternion<double>;
}

