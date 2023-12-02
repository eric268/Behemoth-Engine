#include "MathCore.h"
#include "Misc/Log.h"

// stl
#include <algorithm>

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

	Matrix3x3 Matrix3x3::Identity()
	{
		Matrix3x3 m{};
		for (int i = 0; i < 3; i++)
		{
			m.data[i][i] = 1;
		}
		return m;
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
		assert(list.size() == 4 && list.begin()->size() == 4);

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

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& m) const
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

		float cosTheta = std::cos(theta);
		float sinTheta = std::sin(theta);

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