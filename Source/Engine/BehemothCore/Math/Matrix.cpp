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
		const float x = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
		const float y = m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
		const float z = m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
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

		for (int row = 0; row < n; row++)
		{
			for (int col = 0; col < n; col++)
			{
				float r = std::abs(m1[row][col] - m2[row][col]);
				if (r > epsilon)
					return false;
			}
		}
		return true;
	}

	Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& m)
	{
		Matrix4x4 m2{};
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				m2[col][row] = m[row][col];
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

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				m2[row][col] = Matrix3x3::Determinant(GetSubMatrix(m, row, col));
				m2[row][col] *= ((row + col) % 2 == 0) ? 1.0f : -1.0f;
			}
		}

		m2 = Matrix4x4::Transpose(m2);

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				m2[row][col] /= det;
			}
		}
		
		return m2;
	}

	Matrix3x3 Matrix4x4::GetSubMatrix(const Matrix4x4& m, int skipRow, int skipCol)
	{
		Matrix3x3 m2{};
		int subRow = 0, subCol = 0;

		for (int row = 0; row < 4; row++)
		{
			if (skipRow == row)
				continue;

			subCol = 0;
			for (int col = 0; col < 4; col++)
			{
				if (col == skipCol)
					continue;

				m2[subRow][subCol] = m[row][col];
				subCol++;
			}
			subRow++;
		}
		return m2;
	}

	float Matrix4x4::Determinant(const Matrix4x4& m)
	{
		float det = 0;
		float val = 1.0f;
		for (int i = 0; i < 4; i++)
		{
			det += m[0][i] * Matrix3x3::Determinant(GetSubMatrix(m, 0, i)) * val;
			val *= -1.0f;
		}
		return det;
	}

	// 	Matrix4x4& Matrix4x4::operator= (const Matrix4x4& m)
	// 	{
	// 		for (int i = 0; i < 4; i++)
	// 		{
	// 			for (int j = 0; j < 4; j++)
	// 			{
	// 				data[i][j] = m.data[i][i];
	// 			}
	// 		}
	// 		return *this;
	// 	}

	Vector4 Matrix4x4::operator*(const Vector4 v) const
	{
		// Perform the dot product for each row of the matrix
		Vector4 vec{};

		vec.x = v.x * data[0][0] + v.y * data[1][0] + v.z * data[2][0] + v.w * data[3][0];
		vec.y = v.x * data[0][1] + v.y * data[1][1] + v.z * data[2][1] + v.w * data[3][1];
		vec.z = v.x * data[0][2] + v.y * data[1][2] + v.z * data[2][2] + v.w * data[3][2];
		vec.w = v.x * data[0][3] + v.y * data[1][3] + v.z * data[2][3] + v.w * data[3][3];

		return vec;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m) const
	{
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.data[i][j] = data[i][0] * m.data[0][j] +
					data[i][1] * m.data[1][j] +
					data[i][2] * m.data[2][j] +
					data[i][3] * m.data[3][j];
				
			}
		}
		return result;
	}

	Matrix4x4 Matrix4x4::operator*=(const Matrix4x4& m) const
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
					result.data[i][j] += data[i][k] * m[k][j];
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
			m[1][1] = cosTheta;
			m[1][2] = -sinTheta;
			m[2][1] = sinTheta;
			m[2][2] = cosTheta;
			break;
		case 1:
			m[0][0] = cosTheta;
			m[0][2] = sinTheta;
			m[2][0] = -sinTheta;
			m[2][2] = cosTheta;
			break;
		case 2:
			m[0][0] = cosTheta;
			m[0][1] = -sinTheta;
			m[1][0] = sinTheta;
			m[1][1] = cosTheta;
			break;
		default:
			LOG_ERROR("Invalid axis passed to rotation");
		}
		return m;
	}
}