#pragma once

#include <memory>

namespace Math
{
	class Matrix2x2
	{
	public:
		Matrix2x2() = default;
		~Matrix2x2() = default;

		static Matrix2x2 Identity();

	private:
		float data[2][2];
	};

	class Matrix3x3
	{
	public:
		Matrix3x3() = default;
		~Matrix3x3() = default;

		static Matrix3x3 Identity();

	private:
		float data[3][3];
	};

	class Matrix4x4
	{
	public:
		Matrix4x4() = default;
		~Matrix4x4() = default;

		static Matrix4x4 Identity();

	private:
		float data[4][4];
	};
}


