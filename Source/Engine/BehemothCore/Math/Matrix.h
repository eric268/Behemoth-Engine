#pragma once

#include "Vector.h"
#include "Core/Log.h"

namespace BMath
{
	class BMatrix
	{
	public:
		inline std::size_t Size() const
		{
			return size;
		}
	protected:
		BMatrix(std::size_t s) : size(s) {}

		std::size_t size;
	};

	template <FloatOrDouble T>
	struct BMatrix2x2 : public BMatrix
	{
		union
		{
			T data[2][2];
			struct
			{
				T _11, _21;
				T _12, _22;
			};

		};
		BMatrix2x2() : BMatrix(2), _11((T)0), _12((T)0), _21((T)0), _22((T)0) {}

		static BMatrix2x2 Identity()
		{
			BMatrix2x2 m;
			for (int i = 0; i < 2; i++)
			{
				m.data[i][i] = 0;
			}
			return m;
		}

		static T Determinant(const BMatrix2x2& m)
		{
			return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
		}
	};
	using Matrix2x2  = BMatrix2x2<float>;
	using Matrix2x2d = BMatrix2x2<double>;

	template <FloatOrDouble T>
	struct BMatrix3x3 : public BMatrix
	{
	public:
		union
		{
			T data[3][3];
			struct
			{
				T _11, _21, _31;
				T _12, _22, _32;
				T _13, _23, _33;
			};
		};

		BMatrix3x3() : BMatrix(3)
		{
			std::fill(&data[0][0], &data[0][0] + size * size, (T)0.0);
		}

		BMatrix3x3(std::initializer_list<std::initializer_list<BMatrix3x3>> list) : BMatrix(3)
		{
			int row = 0;
			for (const auto& l : list)
			{
				int col = 0;
				for (const auto val : l)
				{
					data[row][col] = val;
					col++;
				}
				row++;
			}
		}

		static BMatrix3x3 Identity()
		{
			BMatrix3x3 m{};
			for (int i = 0; i < 3; i++)
			{
				m.data[i][i] = (T)1;
			}
			return m;
		}

		BVector3<T> GetColumn(int col)
		{
			return BVector3<T>(data[0][col], data[1][col], data[2][col]);
		}

		BVector3<T> GetRow(int row)
		{
			return BVector3<T>(data[row][0], data[row][1], data[row][2]);
		}

		BVector3<T> operator*(BVector3<T> v)
		{
			BVector3<T> vec{};

			for (int i = 0; i < 3; i++)
			{
				vec.data[i] = v.x * data[i][0] + v.y * data[i][1] + v.z * data[i][2];
			}

			return vec;

		}

		BMatrix3x3 operator*(const BMatrix3x3& m) const
		{
			BMatrix3x3 result{};

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					result.data[i][j] = data[i][0] * m.data[0][j] +
						data[i][1] * m.data[1][j] +
						data[i][2] * m.data[2][j];
				}
			}
			return result;
		}

		static T Determinant(const BMatrix3x3<T>& m)
		{
			const T x = m.data[0][0] * (m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1]);
			const T y = m.data[0][1] * (m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0]);
			const T z = m.data[0][2] * (m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0]);
			return x - y + z;

		}

		static BMatrix2x2<T> GetSubMatrix(const BMatrix3x3<T>& m, int skipCol, int skipRow)
		{
			BMatrix2x2<T> m2{};
			int subRow = 0, subCol = 0;

			for (int i = 0; i < 3; i++)
			{
				if (i == skipCol)
				{
					continue;
				}

				subRow = 0;
				for (int j = 0; j < 3; j++)
				{
					if (j == skipRow)
					{
						continue;
					}

					m2.data[subCol][subRow] = m.data[i][j];
					subRow++;
				}
				subCol++;
			}
			return m2;
		}

		static BMatrix3x3 Inverse(const BMatrix3x3& m)
		{
			BMatrix3x3 m2{};

			T det = BMatrix3x3<T>::Determinant(m);
			if (det == 0.0f)
				return {};

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m2.data[i][j] = BMatrix2x2<T>::Determinant(GetSubMatrix(m, i, j));
					m2.data[i][j] *= ((i + j) % 2 == 0) ? 1.0f : -1.0f;

				}
			}

			m2 = BMatrix3x3<T>::Transpose(m2);

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m2.data[i][j] /= det;
				}
			}

			return m2;
		}

		static BMatrix3x3 Transpose(const BMatrix3x3& m)
		{
			BMatrix3x3 m2{};
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m2.data[i][j] = m.data[j][i];

				}
			}
			return m2;

		}
	};

	using Matrix3x3 = BMatrix3x3<float>;
	using Matrix3x3d = BMatrix3x3<double>;

	template <FloatOrDouble T>
	class BMatrix4x4 : public BMatrix
	{
	public:
		union
		{
			T data[4][4];
			struct
			{
				T _11, _21, _31, _41;
				T _12, _22, _32, _42;
				T _13, _23, _33, _43;
				T _14, _24, _34, _44;
			};
		};

		BMatrix4x4() : BMatrix(4)
		{
			for (int i = 0; i < size; i++)
			{
				std::fill(&data[0][0], &data[0][0] + size * size, (T)0.0);
			}
		}

		BMatrix4x4(std::initializer_list<std::initializer_list<T>> list) : BMatrix(4)
		{
			int i = 0;
			for (const auto& l : list)
			{
				int j = 0;
				for (const T val : l)
				{
					data[i][j] = val;
					j++;
				}
				i++;
			}
		}

		static BMatrix4x4 Zero()
		{
			BMatrix4x4 m{};
			return m;
		}

		static BMatrix4x4 Identity()
		{
			BMatrix4x4 m;
			for (int i = 0; i < 4; i++)
			{
				m.data[i][i] = (T)1;
			}
			return m;
		}

		BVector4<T> GetColumn(int i) const
		{
			return BVector4<T>(data[0][i], data[1][i], data[2][i], data[3][i]);
		}

		BVector4<T> GetRow(int i) const
		{
			return BVector4<T>(data[i][0], data[i][1], data[i][2], data[i][3]);
		}

		static bool Equals(BMatrix4x4<T> m1, BMatrix4x4<T> m2, T epsilon = 1e-2)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					T r = std::abs(m1.data[j][i] - m2.data[j][i]);
					if (r > epsilon)
					{
						return false;
					}
				}
			}
			return true;
		}

		static BMatrix4x4 Transpose(const BMatrix4x4<T>& m)
		{
			BMatrix4x4<T> m2{};
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					m2.data[i][j] = m.data[j][i];

				}
			}
			return m2;
		}

		static BMatrix4x4 Inverse(const BMatrix4x4<T>& m)
		{
			BMatrix4x4<T> m2{};

			T det = BMatrix4x4<T>::Determinant(m);
			if (det == 0.0f)
				return {};

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					m2.data[i][j] = BMatrix3x3<T>::Determinant(GetSubMatrix(m, i, j));
					m2.data[i][j] *= ((i + j) % 2 == 0) ? 1.0f : -1.0f;

				}
			}

			m2 = BMatrix4x4<T>::Transpose(m2);

			for (int col = 0; col < 4; col++)
			{
				for (int row = 0; row < 4; row++)
				{
					m2.data[col][row] /= det;
				}
			}

			return m2;
		}

		static BMatrix3x3<T> GetSubMatrix(const BMatrix4x4& m, int skipCol, int skipRow)
		{
			BMatrix3x3<T> m2{};
			int subRow = 0, subCol = 0;

			for (int i = 0; i < 4; i++)
			{
				if (i == skipCol)
				{
					continue;
				}

				subRow = 0;
				for (int j = 0; j < 4; j++)
				{
					if (j == skipRow)
					{
						continue;
					}

					m2.data[subCol][subRow] = m.data[i][j];
					subRow++;
				}
				subCol++;
			}
			return m2;
		}

		static T Determinant(const BMatrix4x4& m)
		{
			float det = 0;
			float val = 1.0f;
			for (int i = 0; i < 4; i++)
			{
				det += m.data[i][0] * BMatrix3x3<T>::Determinant(GetSubMatrix(m, i, 0)) * val;
				val *= -1.0f;
			}
			return det;
		}

		BVector4<T> operator*(const BVector4<T> v) const
		{
			BVector4<T> vec{};

			for (int i = 0; i < 4; i++)
			{
				vec.data[i] = v.x * data[i][0] + v.y * data[i][1] + v.z * data[i][2] + v.w * data[i][3];
			}

			return vec;
		}

		BMatrix4x4<T> operator*(const BMatrix4x4& m) const
		{
			BMatrix4x4<T> result;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					result.data[i][j] = data[i][0] * m.data[0][j] +
						data[i][1] * m.data[1][j] +
						data[i][2] * m.data[2][j] +
						data[i][3] * m.data[3][j];
				}
			}
			return result;
		}

		BMatrix4x4 operator*=(const BMatrix4x4& m) const
		{
			BMatrix4x4 result;

			const int n = m.Size();
			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					result.data[i][j] = 0;

					for (int k = 0; k < n; k++)
					{
						result.data[i][j] += data[k][j] * m.data[i][k];
					}
				}
			}
			return result;
		}


		static BMatrix4x4 GetRotationMatrix(const int axis, const T theta)
		{
			BMatrix4x4 m = Identity();
			T rad = theta * 3.14159 / 180;

			T cosTheta = std::cos(rad);
			T sinTheta = std::sin(rad);

			switch (axis)
			{
			case 1:
				m._22 = cosTheta;
				m._23 = -sinTheta;
				m._32 = sinTheta;
				m._33 = cosTheta;
				break;
			case 2:
				m._11 = cosTheta;
				m._13 = -sinTheta;
				m._31 = sinTheta;
				m._33 = cosTheta;
				break;
			case 3:
				m._11 = cosTheta;
				m._12 = -sinTheta;
				m._21 = sinTheta;
				m._22 = cosTheta;
				break;
			default:
				LOGMESSAGE(Warning, "Invalid axis passed to rotation");
			}
			return m;
		}
	};

	using Matrix4x4  = BMatrix4x4<float>;
	using Matrix4x4d = BMatrix4x4<double>;
}


