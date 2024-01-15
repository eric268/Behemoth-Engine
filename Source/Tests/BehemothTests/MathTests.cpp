#include "pch.h"

//Math Tests

// TEST(Name, n2)
// {
// 	EXPECT_TRUE(true);
// }
// 
//  TEST(Vec4_Mul_M4x4, BMath)
//  {
//  	BMath::Vector4 vec(10.0, -8.0, 7.72, -0.1);
//  
//  	BMath::Matrix4x4d m =
//  	{
//  		{   9.0 ,   0.3 ,    7.0, -12.2},
//  		{ -19.0 ,   0.87, -107.1,   0.0},
//  		{  32.33, -10.12,  -72.5, -12.2},
//  		{  91.0 ,   1.13,   -7.7,  92.72}
//  	};
//  
//  	BMath::Vector4 result = m * vec;
//  	BMath::Vector4 ans(142.86, -1023.772, -154.22, 832.244);
//  
//  	EXPECT_TRUE(BMath::Vector4::Equals(result, ans));
//  }
// 
// TEST(Vec4_Mul_M4x4_T2, BMath)
// {
// 	BMath::Vector4 vec(1.0, 1.0, -1.0, 1.0);
// 
// 	BMath::Matrix4x4d m =
// 	{
// 		{  1.0 ,   0.0 ,  0.0,  0.0},
// 		{  0.0 ,   1.0,   0.0,  0.0},
// 		{  0.0,	   0.0,   1.0, -5.0},
// 		{  0.0 ,   0.0,   0.0,  1.0}
// 	};
// 
// 	BMath::Vector4 result = m * vec;
// 	BMath::Vector4 ans(1.0, 1.0, -6.0, 1.0);
// 
// 	EXPECT_TRUE(BMath::Vector4::Equals(result, ans));
// }
// 
// TEST(M4x4_Mul_M4x4, BMath)
// {
// 	BMath::Matrix4x4d m1 =
// 	{
// 		{   12.3 ,      10.13,     5.15,   2.2} ,
// 		{   -9.0,     1020.0,   -107.1 ,   0.0} ,
// 		{  20.33,      -0.12,     -2.5 , 115.17},
// 		{ -61.12,       0.0 ,    -16.1 ,   0.72}
// 	};
// 
// 	BMath::Matrix4x4d m2 =
// 	{
// 		{   6.1 , 10.31,     0.0 , -1003.145},
// 		{  11.0 ,  1.87,   -17.11,     0.0}  ,
// 		{ -12.11,  9.12,   -72.5 ,   -12.223},
// 		{ 191.0 ,  1.0 ,    -0.7 ,     2.72}
// 	};
// 
// 	BMath::Matrix4x4d result = m1 * m2;
// 
// 	BMath::Matrix4x4d ans =
// 	{
// 		{544.2935, 194.9241, -548.2393, -12395.64795},
// 		{12462.081, 837.858, -9687.45, 10337.3883 },
// 		{22150.438, 301.7479, 102.6842, -20050.11795},
// 		{-40.341, -776.2592, 1166.746, 61510.9711}
// 	};
// 
// 	EXPECT_TRUE(BMath::Matrix4x4d::Equals(result, ans));
// }
// 
// TEST(CrossProd, BMath)
// {
// 	BMath::Vector3 vec1(10.12, -99.76, 0.13);
// 	BMath::Vector3 vec2(-1.45, 20.25, -102.12);
// 
// 	BMath::Vector3 result = BMath::Vector3::Cross(vec1, vec2);
// 	BMath::Vector3 ans(10184.8587, 1033.2659, 60.278);
// 
// 	EXPECT_TRUE(BMath::Vector3::Equals(result, ans));
// }
// 
// TEST(Transpose_M4x4, BMath)
// {
// 	BMath::Matrix4x4d m1 =
// 	{
// 		{1.0, 10.0, 11.0, 5.0},
// 		{-2.0, 3.0, 5.0, 17.0},
// 		{0.0, 9.0, 2.0, -2.0},
// 		{-4.0, -7.0, 11.0, 61.0}
// 	};
// 
// 	BMath::Matrix4x4d result = BMath::Matrix4x4d::Transpose(m1);
// 
// 	BMath::Matrix4x4d ans =
// 	{
// 		{1.0, -2.0, 0.0, -4.0},
// 		{10.0, 3.0, 9.0, -7.0},
// 		{11.0, 5.0, 2.0, 11.0},
// 		{5.0, 17.0, -2.0, 61.0}
// 	};
// 
// 	EXPECT_TRUE(BMath::Matrix4x4d::Equals(result, ans));
// }
// 
// TEST(Inverse_M4x4, BMath)
// {
// 	BMath::Matrix4x4d m1 =
// 	{
// 		{1.0, 10.0, 11.0, 5.0},
// 		{-2.0, 3.0, 5.0, 17.0},
// 		{0.0, 9.0, 2.0, -2.0},
// 		{-4.0, -7.0, 11.0, 61.0}
// 	};
// 
// 	BMath::Matrix4x4d result = BMath::Matrix4x4d::Inverse(m1);
// 
// 	BMath::Matrix4x4d ans =
// 	{
// 		{ 0.0888  , -1.07  , 0.4967 ,  0.3073 },
// 		{ -0.03089, -0.075 , 0.1936 ,  0.0298 },
// 		{ 0.1197  ,  0.2192, -0.2681, -0.0797 },
// 		{ -0.0193 , -0.1183,  0.1031,  0.0543 }
// 	};
// 
// 	EXPECT_TRUE(BMath::Matrix4x4d::Equals(result, ans));
// }
// 
// TEST(Inverse_M4x4_2, BMath)
// {
// 	BMath::Matrix4x4d m1 =
// 	{
// 		{1.0, 0.0, 0.0, 0.0},
// 		{0.0, 1.0, 0.0, 0.0},
// 		{0.0, 0.0, 1.0, -10.0},
// 		{0.0, 0.0, 0.0, 1.0}
// 	};
// 
// 	BMath::Matrix4x4d result = BMath::Matrix4x4d::Inverse(m1);
// 
// 	BMath::Matrix4x4d ans =
// 	{
// 		{1.0, 0.0, 0.0, 0.0},
// 		{0.0, 1.0, 0.0, 0.0},
// 		{0.0, 0.0, 1.0, 10.0},
// 		{0.0, 0.0, 0.0, 1.0}
// 	};
// 
// 	EXPECT_TRUE(BMath::Matrix4x4d::Equals(result, ans));


TEST(Quaternion_Axis_Angle, BMath)
{
	BMath::Quaternion q1(0.785, BMath::Vector3(1.0, 1.0, 1.0));
	BMath::Quaternion q2(0.392, BMath::Vector3(0.0, 0.707, 0.707));

	BMath::Quaternion a1(0.924, 0.221, 0.221, 0.221);
	BMath::Quaternion a2(0.981, 0.0, 0.138, 0.138);

	EXPECT_TRUE(BMath::Quaternion::Equals(q1, a1) && BMath::Quaternion::Equals(q2, a2));
}

TEST(Quaternion_Mult, BMath)
{
	BMath::Quaternion q(0.98, 0.13, 0.14, 0.0);
	BMath::Quaternion q2(0.92, 0.0, 0.0, 0.38);

	BMath::Quaternion result = q * q2;
	BMath::Quaternion ans(0.9016, 0.1728, 0.0794, 0.3724);


	EXPECT_TRUE(BMath::Quaternion::Equals(result, ans));
}

TEST(Quaternion_Mult_2, BMath)
{
	BMath::Quaternion q(0.88, BMath::Vector3(1.0, 1.5, 0.3));
	BMath::Quaternion q2(0.15, BMath::Vector3(0.17, 0.27, 0.99));

	BMath::Quaternion result = q * q2;
	BMath::Quaternion ans(0.8879, 0.2667, 0.3504, 0.1343);
	EXPECT_TRUE(BMath::Quaternion::Equals(result, ans));
}