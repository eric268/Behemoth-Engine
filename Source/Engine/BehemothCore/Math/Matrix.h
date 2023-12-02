#pragma once
// stl
#include <memory>
#include <cassert>

namespace Math
{
	// Want to make this a pure virtual class eventually
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

	class Matrix2x2 : public Matrix
	{
	public:
		Matrix2x2() : Matrix(2) {};
		~Matrix2x2() = default;

		static Matrix2x2 Identity();
		float data[2][2];

	private:
	};

	class Matrix3x3 : public Matrix
	{
	public:
		Matrix3x3();
		~Matrix3x3() = default;

		static Matrix3x3 Identity();

		float data[3][3];

		float* operator[](int row)
		{
			assert(row < 3 && row >= 0);
			return data[row];
		}
		const float* operator[](int row) const
		{
			assert(row < 3 && row >= 0);
			return data[row];
		}

	private:
	};

	class Matrix4x4 : public Matrix
	{
	public:
		Matrix4x4();
		Matrix4x4(std::initializer_list<std::initializer_list<float>> list);
		~Matrix4x4() = default;

		static Matrix4x4 Zero();
		static Matrix4x4 Identity();
		static bool Equals(Matrix4x4 m1, Matrix4x4 m2, float epsilon = 1e-2);
		static Matrix4x4 GetRotationMatrix(const int axis, const float theta);

		float data[4][4];

		// Matrix4x4& operator= (const Matrix4x4& m);
		Matrix4x4 operator*(const Matrix4x4& m) const;
		Matrix4x4 operator*=(const Matrix4x4& m) const;

		float* operator[](int row)
		{
			assert(row < 4 && row >=0);
			return data[row];
		}
		const float* operator[](int row) const
		{
			assert(row < 4 && row >= 0);
			return data[row];
		}

	private:
	};
}


