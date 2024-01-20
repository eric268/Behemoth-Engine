#pragma once

#include "Vector.h"
#include "Core/Log.h"

namespace BMath
{
	class Matrix
	{
	public:
		inline std::size_t Size() const
		{
			return size;
		}
	protected:
		Matrix(std::size_t s) : size(s) {}

		std::size_t size;
	};

	template <FloatOrDouble T = float>
	class Matrix2x2 : public Matrix
	{
	public:
		Matrix2x2() : Matrix(2), _11((T)0), _12((T)0), _21((T)0), _22((T)0) {}

		static Matrix2x2 Identity()
		{
			Matrix2x2 m;
			for (int i = 0; i < 2; i++)
			{
				m.data[i][i] = 0;
			}
			return m;
		}

		static T Determinant(const Matrix2x2& m)
		{
			return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
		}

		union
		{
			T data[2][2];
			struct
			{
				T _11, _21;
				T _12, _22;
			};
		};
	};
	using BMatrix2x2 = Matrix2x2<float>;
	using BMatrix2x2d = Matrix2x2<double>;

	template <FloatOrDouble T = float>
	class Matrix3x3 : public Matrix
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

		// Matrix3x3
		Matrix3x3() : Matrix(3)
		{
			std::fill(&data[0][0], &data[0][0] + size * size, (T)0.0);
		}

		Matrix3x3(std::initializer_list<std::initializer_list<Matrix3x3>> list) : Matrix(3)
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

		static Matrix3x3 Identity()
		{
			Matrix3x3 m{};
			for (int i = 0; i < 3; i++)
			{
				m.data[i][i] = (T)1;
			}
			return m;
		}

		Vector3 GetColumn(int col)
		{
			return Vector3(data[0][col], data[1][col], data[2][col]);
		}

		Vector3 GetRow(int row)
		{
			return Vector3(data[row][0], data[row][1], data[row][2]);
		}

		Vector3 operator*(Vector3 v)
		{
			Vector3 vec{};

			for (int i = 0; i < 3; i++)
			{
				vec.data[i] = v.x * data[i][0] + v.y * data[i][1] + v.z * data[i][2];
			}

			return vec;

		}

		Matrix3x3 operator*(const Matrix3x3& m) const
		{
			Matrix3x3 result{};

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

		static T Determinant(const Matrix3x3& m)
		{
			const T x = m.data[0][0] * (m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1]);
			const T y = m.data[0][1] * (m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0]);
			const T z = m.data[0][2] * (m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0]);
			return x - y + z;

		}

		static Matrix2x2<T> GetSubMatrix(const Matrix3x3& m, int skipCol, int skipRow)
		{
			Matrix2x2<T> m2{};
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

		static Matrix3x3 Inverse(const Matrix3x3& m)
		{
			Matrix3x3 m2{};

			T det = Matrix3x3::Determinant(m);
			if (det == 0.0f)
				return {};

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m2.data[i][j] = Matrix2x2<T>::Determinant(GetSubMatrix(m, i, j));
					m2.data[i][j] *= ((i + j) % 2 == 0) ? 1.0f : -1.0f;

				}
			}

			m2 = Matrix3x3::Transpose(m2);

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m2.data[i][j] /= det;
				}
			}

			return m2;
		}

		static Matrix3x3 Transpose(const Matrix3x3& m)
		{
			Matrix3x3 m2{};
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

	using BMatrix3x3 = Matrix3x3<float>;
	using BMatrix3x3d = Matrix3x3<double>;

	template <FloatOrDouble T = double>
	class Matrix4x4 : public Matrix
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

		Matrix4x4() : Matrix(4)
		{
			for (int i = 0; i < size; i++)
			{
				std::fill(&data[0][0], &data[0][0] + size * size, (T)0.0);
			}
		}

		Matrix4x4(std::initializer_list<std::initializer_list<T>> list) : Matrix(4)
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

		static Matrix4x4 Zero()
		{
			Matrix4x4 m{};
			return m;
		}

		static Matrix4x4 Identity()
		{
			Matrix4x4 m;
			for (int i = 0; i < 4; i++)
			{
				m.data[i][i] = (T)1;
			}
			return m;
		}

		Vector4 GetColumn(int i) const
		{
			return Vector4(data[0][i], data[1][i], data[2][i], data[3][i]);
		}

		Vector4 GetRow(int i) const
		{
			return Vector4(data[i][0], data[i][1], data[i][2], data[i][3]);
		}

		static bool Equals(Matrix4x4<T> m1, Matrix4x4<T> m2, T epsilon = 1e-2)
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

		static Matrix4x4 Transpose(const Matrix4x4<T>& m)
		{
			Matrix4x4<T> m2{};
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					m2.data[i][j] = m.data[j][i];

				}
			}
			return m2;
		}

		static Matrix4x4 Inverse(const Matrix4x4<T>& m)
		{
			Matrix4x4<T> m2{};

			T det = Matrix4x4<T>::Determinant(m);
			if (det == 0.0f)
				return {};

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					m2.data[i][j] = Matrix3x3<T>::Determinant(GetSubMatrix(m, i, j));
					m2.data[i][j] *= ((i + j) % 2 == 0) ? 1.0f : -1.0f;

				}
			}

			m2 = Matrix4x4<T>::Transpose(m2);

			for (int col = 0; col < 4; col++)
			{
				for (int row = 0; row < 4; row++)
				{
					m2.data[col][row] /= det;
 				}
 			}

			return m2;
		}

		static Matrix3x3<T> GetSubMatrix(const Matrix4x4& m, int skipCol, int skipRow)
		{
			Matrix3x3<T> m2{};
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

		static T Determinant(const Matrix4x4& m)
		{
			float det = 0;
			float val = 1.0f;
			for (int i = 0; i < 4; i++)
			{
				det += m.data[i][0] * Matrix3x3<T>::Determinant(GetSubMatrix(m, i, 0)) * val;
				val *= -1.0f;
			}
			return det;
		}

		Vector4 operator*(const Vector4 v) const
		{
			Vector4 vec{};

			for (int i = 0; i < 4; i++)
			{
				vec.data[i] = v.x * data[i][0] + v.y * data[i][1] + v.z * data[i][2] + v.w * data[i][3];
			}

			return vec;
		}

		Matrix4x4<T> operator*(const Matrix4x4& m) const
		{
			Matrix4x4<T> result;

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

		Matrix4x4 operator*=(const Matrix4x4& m) const
		{
			Matrix4x4 result;

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


		static Matrix4x4 GetRotationMatrix(const int axis, const T theta)
		{
			Matrix4x4 m = Identity();
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

	using BMatrix4x4 = Matrix4x4<float>;
	using BMatrix4x4d = Matrix4x4<double>;
}


