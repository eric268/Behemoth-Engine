#include "pch.h"
#include "MathCore.h"
#include "Misc/Log.h"


// stl
#include <algorithm>

#include <xmmintrin.h> 

namespace Math
{
	// Matrix2x2
	Matrix2x2 Matrix2x2::Identity()
	{
		Matrix2x2 m;
		for (int i = 0; i < 2; i++)
		{
			m.data[i][i] = 0;
		}
		return m;
	}

	// Matrix3x3
	Matrix3x3::Matrix3x3() : Matrix(3)
	{
		std::fill(&data[0][0], &data[0][0] + size * size, 0.0f);
	}

	Matrix3x3::Matrix3x3(std::initializer_list<std::initializer_list<float>> list) : Matrix(3)
	{
		*this = Matrix::GetMatrixFromList<Matrix3x3>(list, size);
	}

	Matrix3x3 Matrix3x3::Identity()
	{
		Matrix3x3 m{};
		for (int i = 0; i < 3; i++)
		{
			m.data[i][i] = 1;
		}
		return m;
	}

	float Matrix3x3::Determinant(const Matrix3x3& m)
	{
		const float x = m.data[0][0] * (m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1]);
		const float y = m.data[0][1] * (m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0]);
		const float z = m.data[0][2] * (m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0]);
		return x - y + z;

	}

	//Matrix 4x4
	Matrix4x4::Matrix4x4() : Matrix(4)
	{
		for (int i = 0; i < size; i++)
		{
			std::fill(&data[0][0], &data[0][0] + size * size, 0);
		}
	}

	Matrix4x4::Matrix4x4(std::initializer_list<std::initializer_list<float>> list) : Matrix(4)
	{
		*this = Matrix::GetMatrixFromList<Matrix4x4>(list, size);
	}

	Matrix4x4 Matrix4x4::Zero()
	{
		Matrix4x4 m;
		return m;
	}

	Matrix4x4 Matrix4x4::Identity()
	{
		Matrix4x4 m;
		for (int i = 0; i < 4; i++)
		{
			m.data[i][i] = 1;
		}
		return m;
	}

	bool Matrix4x4::Equals(Matrix4x4 m1, Matrix4x4 m2, float epsilon)
	{
		int n = m1.Size();
		int m = m2.Size();
		if (m != 4 || n != 4)
			return false;

		for (int col = 0; col < n; col++)
		{
			for (int row = 0; row < n; row++)
			{
				float r = std::abs(m1.data[row][col] - m2.data[row][col]);
				if (r > epsilon)
					return false;

			}
		}
		return true;
	}

	Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& m)
	{
		Matrix4x4 m2{};
		for (int col = 0; col < 4; col++)
		{
			for (int row = 0; row < 4; row++)
			{
				m2.data[col][row] = m.data[row][col];

			}
		}
		return m2;
	}
	Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& m)
	{
		Matrix4x4 m2{};

		float det = Matrix4x4::Determinant(m);
		if (det == 0.0f)
			return {};

		for (int col = 0; col < 4; col++)
		{
			for (int row = 0; row < 4; row++)
			{
				m2.data[col][row] = Matrix3x3::Determinant(GetSubMatrix(m, col, row));
				m2.data[col][row] *= ((col + row) % 2 == 0) ? 1.0f : -1.0f;

			}
		}

		m2 = Matrix4x4::Transpose(m2);

		for (int col = 0; col < 4; col++)
		{
			for (int row = 0; row < 4; row++)
			{
				m2.data[col][row] /= det;
			}
		}
		
		return m2;
	}

	Matrix3x3 Matrix4x4::GetSubMatrix(const Matrix4x4& m, int skipCol, int skipRow)
	{
		Matrix3x3 m2{};
		int subRow = 0, subCol = 0;

		for (int col = 0; col < 4; col++) 
		{
			if (col == skipCol)
				continue;

			subRow = 0;
			for (int row = 0; row < 4; row++) 
			{
				if (row == skipRow)
					continue;

				m2.data[subCol][subRow] = m.data[col][row];
				subRow++;
			}
			subCol++;
		}
		return m2;
	}

	float Matrix4x4::Determinant(const Matrix4x4& m)
	{
		float det = 0;
		float val = 1.0f;
		for (int i = 0; i < 4; i++)
		{
			det += m.data[i][0] * Matrix3x3::Determinant(GetSubMatrix(m, i, 0)) * val;
			val *= -1.0f;
		}
		return det;
	}

	Vector4 Matrix4x4::operator*(const Vector4 v) const
	{
		// Perform the dot product for each row of the matrix
		Vector4 vec{};

		for (int i = 0; i < 4; i++) 
		{
			vec.data[i] = v.x * data[i][0] + v.y * data[i][1] + v.z * data[i][2] + v.w * data[i][3];
		}

		return vec;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m) const
	{
		Matrix4x4 result;

		for (int col = 0; col < 4; col++) 
		{
			for (int row = 0; row < 4; row++) 
			{
				result.data[col][row] = data[col][0] * m.data[0][row] +
										data[col][1] * m.data[1][row] +
										data[col][2] * m.data[2][row] +
										data[col][3] * m.data[3][row];
			}
		}
		return result;
	}

	Matrix4x4 Matrix4x4::operator*=(const Matrix4x4& m) const 
	{
		Matrix4x4 result;

		const int n = m.Size();
		for (int col = 0; col < n; col++) 
		{
			for (int row = 0; row < n; row++) 
			{
				result.data[col][row] = 0;

				for (int k = 0; k < n; k++) 
				{
					result.data[col][row] += data[k][row] * m.data[col][k];
				}
			}
		}
		return result;
	}


	Matrix4x4 Matrix4x4::GetRotationMatrix(const int axis, const float theta)
	{
		Matrix4x4 m = Matrix4x4::Identity();
		float rad = DEGREE_TO_RAD(theta);

		float cosTheta = std::cos(rad);
		float sinTheta = std::sin(rad);

		switch (axis)
		{
		case 0:
			m._22 = cosTheta;
			m._23 = -sinTheta;
			m._32 = sinTheta;
			m._33 = cosTheta;
			break;
		case 1:
			m._11 = cosTheta;
			m._13 = -sinTheta;
			m._31 = sinTheta;
			m._33 = cosTheta;
			break;
		case 2:
			m._11 = cosTheta;
			m._12 = -sinTheta;
			m._21 = sinTheta;
			m._22 = cosTheta;
			break;
		default:
			LOG_ERROR("Invalid axis passed to rotation");
		}
		return m;
	}
}