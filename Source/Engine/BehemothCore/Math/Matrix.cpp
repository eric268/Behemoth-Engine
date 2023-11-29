#include "Matrix.h"

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
	Matrix3x3 Matrix3x3::Identity()
	{
		Matrix3x3 m;
		for (int i = 0; i < 3; i++)
		{
			m.data[i][i] = 0;
		}
		return m;
	}




	//Matrix 4x4
	Matrix4x4 Matrix4x4::Identity()
	{
		Matrix4x4 m;
		for (int i = 0; i < 4; i++)
		{
			m.data[i][i] = 0;
		}
		return m;
	}
}