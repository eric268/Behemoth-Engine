#include "pch.h"

//Math Tests

TEST(Name, n2)
{
	EXPECT_TRUE(true);
}

 TEST(Vec4_Mul_M4x4, BMath)
 {
 	BMath::Vector4 vec(10.0f, -8.0f, 7.72f, -0.1f);

 	BMath::Matrix4x4f m =
 	{
 		{   9.0f ,   0.3f ,    7.0f, -12.2f},
 		{ -19.0f ,   0.87f, -107.1f,   0.0f},
 		{  32.33f, -10.12f,  -72.5f, -12.2f},
 		{  91.0f ,   1.13f,   -7.7f,  92.72f}
 	};
 
 	BMath::Vector4 result = m * vec;
 	BMath::Vector4 ans(142.86f, -1023.772f, -154.22f, 832.244f);
 
 	EXPECT_TRUE(BMath::Vector4::Equals(result, ans));
 }
 
 TEST(Vec4_Mul_M4x4_T2, BMath)
 {
 	BMath::Vector4 vec(1.0f, 1.0f, -1.0f, 1.0f);
 
 	BMath::Matrix4x4f m =
 	{
		{  1.0f ,   0.0f ,  0.0f,  0.0f},
		{  0.0f ,   1.0f,   0.0f,  0.0f},
		{  0.0f,	0.0f,   1.0f, -5.0f},
 		{  0.0f ,   0.0,    0.0f,  1.0f}
 	};
 
 	BMath::Vector4 result = m * vec;
 	BMath::Vector4 ans(1.0f, 1.0f, -6.0f, 1.0f);
 
 	EXPECT_TRUE(BMath::Vector4::Equals(result, ans));
 }
  
  TEST(M4x4_Mul_M4x4, BMath)
  {
  	BMath::Matrix4x4f m1 =
  	{
 		{   12.3f ,      10.13f,     5.15f,   2.2f} ,
 		{   -9.0f,     1020.0f,   -107.1f ,   0.0f} ,
 		{  20.33f,      -0.12f,     -2.5f , 115.17f},
 		{ -61.12f,       0.0f ,    -16.1f ,   0.72f}
  	};
  
  	BMath::Matrix4x4f m2 =
  	{
 		{   6.1f , 10.31f,     0.0f , -1003.145f},
 		{  11.0f ,  1.87f,   -17.11f,     0.0f}  ,
 		{ -12.11f,  9.12f,   -72.5f ,   -12.223f},
  		{ 191.0f ,  1.0f ,    -0.7f ,     2.72f}
  	};
  
  	BMath::Matrix4x4f result = m1 * m2;
  
  	BMath::Matrix4x4f ans =
  	{
  		{544.2935f, 194.9241f, -548.2393f, -12395.64795f},
  		{12462.081f, 837.858f, -9687.45f, 10337.3883f },
  		{22150.438f, 301.7479f, 102.6842f, -20050.11795f},
  		{-40.341f, -776.2592f, 1166.746f, 61510.9711f}
  	};
  
  	EXPECT_TRUE(BMath::Matrix4x4f::Equals(result, ans));
  }
  
  TEST(CrossProd, BMath)
  {
  	BMath::Vector3 vec1(10.12f, -99.76f, 0.13f);
  	BMath::Vector3 vec2(-1.45f, 20.25f, -102.12f);
  
  	BMath::Vector3 result = BMath::Vector3::Cross(vec1, vec2);
  	BMath::Vector3 ans(10184.8587f, 1033.2659f, 60.278f);
  
  	EXPECT_TRUE(BMath::Vector3::Equals(result, ans));
  }
  
  TEST(Transpose_M4x4, BMath)
  {
  	BMath::Matrix4x4f m1 =
  	{
  		{1.0f, 10.0f, 11.0f, 5.0f},
  		{-2.0f, 3.0f, 5.0f, 17.0f},
  		{0.0f, 9.0f, 2.0f, -2.0f},
  		{-4.0f, -7.0f, 11.0f, 61.0f}
  	};
  
  	BMath::Matrix4x4f result = BMath::Matrix4x4f::Transpose(m1);
  
  	BMath::Matrix4x4f ans =
  	{
 		{1.0f, -2.0f,  0.0f, -4.0f},
 		{10.0f, 3.0f,  9.0f, -7.0f},
 		{11.0f, 5.0f,  2.0f, 11.0f},
 		{5.0f, 17.0f, -2.0f, 61.0f}
  	};
  
  	EXPECT_TRUE(BMath::Matrix4x4f::Equals(result, ans));
  }
  
  TEST(Inverse_M4x4, BMath)
  {
  	BMath::Matrix4x4f m1 =
  	{
 		{ 1.0f, 10.0f, 11.0f,  5.0f},
 		{-2.0f,  3.0f,  5.0f, 17.0f},
 		{ 0.0f,  9.0f,  2.0f, -2.0f},
 		{-4.0f, -7.0f, 11.0f, 61.0f}
  	};
  
  	BMath::Matrix4x4f result = BMath::Matrix4x4f::Inverse(m1);
  
  	BMath::Matrix4x4f ans =
  	{
  		{ 0.0888f  , -1.07f  , 0.4967f ,  0.3073f },
  		{ -0.03089f, -0.075f , 0.1936f ,  0.0298f },
  		{ 0.1197f  ,  0.2192f, -0.2681f, -0.0797f },
  		{ -0.0193f , -0.1183f,  0.1031f,  0.0543f }
  	};
  
  	EXPECT_TRUE(BMath::Matrix4x4f::Equals(result, ans));
  }
  
  TEST(Inverse_M4x4_2, BMath)
  {
      BMath::Matrix4x4f m1 =
      {
 		 {1.0f, 0.0f, 0.0f, 0.0f},
 		 {0.0f, 1.0f, 0.0f, 0.0f},
 		 {0.0f, 0.0f, 1.0f, -10.0f},
 		 {0.0f, 0.0f, 0.0f, 1.0f}
      };
 
      BMath::Matrix4x4f result = BMath::Matrix4x4f::Inverse(m1);
 
      BMath::Matrix4x4f ans =
      {
 		 {1.0f, 0.0f, 0.0f, 0.0f},
 		 {0.0f, 1.0f, 0.0f, 0.0f},
 		 {0.0f, 0.0f, 1.0f, 10.0f},
 		 {0.0f, 0.0f, 0.0f, 1.0f}
      };
 
      EXPECT_TRUE(BMath::Matrix4x4f::Equals(result, ans));
  }


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