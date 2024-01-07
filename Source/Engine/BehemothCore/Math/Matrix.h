#pragma once

#include "Vector.h"
// stl
#include <memory>
#include <cassert>

namespace BMath
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

		template <typename T>
			requires std::is_base_of_v<Matrix, T>
		static T GetMatrixFromList(std::initializer_list<std::initializer_list<float>> list, std::size_t size)
		{
			T matrix{};
			assert(size == matrix.size && list.size() == size && list.begin()->size() == size);

			int row = 0;
			for (const auto& l : list)
			{
				int col = 0;
				for (const auto val : l)
				{
					matrix.data[row][col] = val;
					col++;
				}
				row++;
			}
			return matrix;
		}

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
		Matrix3x3(std::initializer_list<std::initializer_list<float>> list);
		~Matrix3x3() = default;

		union
		{
			float data[3][3];
			struct
			{
				float _11, _21, _31;
				float _12, _22, _32;
				float _13, _23, _33;
			};
		};

		static Matrix3x3 Identity();
		static float Determinant(const Matrix3x3& m);

	private:
	};

	class Matrix4x4 : public Matrix
	{
	public:
		Matrix4x4();
		Matrix4x4(std::initializer_list<std::initializer_list<float>> list);
		~Matrix4x4() = default;

		union
		{
			float data[4][4];
			struct
			{
				float _11, _21, _31, _41;
				float _12, _22, _32, _42;
				float _13, _23, _33, _43;
				float _14, _24, _34, _44;
			};
		};

		static Matrix4x4 Zero();
		static Matrix4x4 Identity();
		static bool Equals(Matrix4x4 m1, Matrix4x4 m2, float epsilon = 1e-2);
		static Matrix4x4 GetRotationMatrix(const int axis, const float theta);
		static Matrix4x4 Transpose(const Matrix4x4& m);
		static Matrix4x4 Inverse(const Matrix4x4& m);
		static Matrix3x3 GetSubMatrix(const Matrix4x4& m, int skipRow, int skipCol);
		static float Determinant(const Matrix4x4& m);

		// Matrix4x4& operator= (const Matrix4x4& m);
		Matrix4x4 operator*(const Matrix4x4& m) const;
		Matrix4x4 operator*=(const Matrix4x4& m) const;

		Vector4 operator*(const Vector4 vec) const;

	private:
	};
}


