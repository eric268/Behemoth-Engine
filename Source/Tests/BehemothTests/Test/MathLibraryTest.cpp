#include "pch.h"

//Math Tests
TEST(Vec4_Mul_M4x4, Math)
{
	Math::Vector4 vec(10.0f, -8.0f, 7.72f, -0.1f);

	Math::Matrix4x4 m =
	{
		{   9.0f ,   0.3f ,    7.0f, -12.2f},
		{ -19.0f ,   0.87f, -107.1f,   0.0f},
		{  32.33f, -10.12f,  -72.5f, -12.2f},
		{  91.0f ,   1.13f,   -7.7f,  92.72f}
	};

	Math::Vector4 result = m * vec;
	Math::Vector4 ans(142.86, -1023.772, -154.22f, 832.244f);

	EXPECT_TRUE(Math::Vector4::Equals(result, ans));
}

TEST(Vec4_Mul_M4x4_T2, Math)
{
	Math::Vector4 vec(1.0f, 1.0f, -1.0f, 1.0f);

	Math::Matrix4x4 m =
	{
		{  1.0f ,   0.0f ,  0.0f,  0.0f},
		{  0.0f ,   1.0f,   0.0f,  0.0f},
		{  0.0f,	0.0f,   1.0f, -5.0f},
		{  0.0f ,   0.0f,   0.0f,  1.0f}
	};

	Math::Vector4 result = m * vec;
	Math::Vector4 ans(1.0f, 1.0f, -6.0f, 1.0f);

	EXPECT_TRUE(Math::Vector4::Equals(result, ans));
}

TEST(M4x4_Mul_M4x4, Math)
{
	Math::Matrix4x4 m1 =
	{
		{  12.3f ,   10.13f,    5.15f,   2.2f} ,
		{   -9.0f, 1020.0f , -107.1f ,   0.0f} ,
		{  20.33f,   -0.12f,   -2.5f , 115.17f},
		{ -61.12f,    0.0f ,  -16.1f ,   0.72f}
	};

	Math::Matrix4x4 m2 =
	{
		{   6.1f , 10.31f,     0.0f , -1003.145f},
		{  11.0f ,  1.87f,   -17.11f,     0.0f}  ,
		{ -12.11f,  9.12f,   -72.5f ,   -12.223f},
		{ 191.0f ,  1.0f ,    -0.7f ,     2.72f}
	};

	Math::Matrix4x4 result = m1 * m2;

	Math::Matrix4x4 ans =
	{
		{544.2935f, 194.9241f, -548.2393f, -12395.64795f},
		{12462.081f, 837.858f, -9687.45, 10337.3883 },
		{22150.438, 301.7479, 102.6842, -20050.11795},
		{-40.341, -776.2592, 1166.746, 61510.9711}
	};

	EXPECT_TRUE(Math::Matrix4x4::Equals(result, ans));
}

TEST(CrossProd, Math)
{
	Math::Vector3 vec1(10.12f, -99.76, 0.13);
	Math::Vector3 vec2(-1.45, 20.25, -102.12);

	Math::Vector3 result = Math::Vector3::Cross(vec1, vec2);
	Math::Vector3 ans(10184.8587, 1033.2659, 60.278);

	EXPECT_TRUE(Math::Vector3::Equals(result, ans));
}

TEST(TransposeM4x4, Math)
{
	Math::Matrix4x4 m1 =
	{
		{1, 10, 11, 5},
		{-2, 3, 5, 17},
		{0, 9, 2, -2},
		{-4, -7, 11, 61}
	};

	Math::Matrix4x4 result = Math::Matrix4x4::Transpose(m1);

	Math::Matrix4x4 ans =
	{
		{1, -2, 0, -4},
		{10, 3, 9, -7},
		{11, 5, 2, 11},
		{5, 17, -2, 61}
	};

	EXPECT_TRUE(Math::Matrix4x4::Equals(result, ans));
}

TEST(InverseM4x4, Math)
{
	Math::Matrix4x4 m1 =
	{
		{1, 10, 11, 5},
		{-2, 3, 5, 17},
		{0, 9, 2, -2},
		{-4, -7, 11, 61}
	};

	Math::Matrix4x4 result = Math::Matrix4x4::Inverse(m1);

	Math::Matrix4x4 ans =
	{
		{ 0.0888f  , -1.07f  , 0.4967f ,  0.3073f },
		{ -0.03089f, -0.075f , 0.1936f ,  0.0298f },
		{ 0.1197f  ,  0.2192f, -0.2681f, -0.0797f },
		{ -0.0193f , -0.1183f,  0.1031f,  0.0543f }
	};

	EXPECT_TRUE(Math::Matrix4x4::Equals(result, ans));
}