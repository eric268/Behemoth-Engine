#include "pch.h"

// Math Tests
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

	Math::Vector4 result = vec * m;
	Math::Vector4 ans(482.4876f, -82.1994, 367.87f, -225.456);

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