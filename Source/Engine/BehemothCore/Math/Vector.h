#pragma once

namespace Math
{
// 	template<typename T>
// 	concept VecParam = std::is_integral_v<T> || std::is_floating_point_v<T>;
// 
// 	template <typename T> requires VecParam<T>

	class Vector2
	{
	public:
		Vector2() = default;
		Vector2(const float x, const float y);
		
		float x;
		float y;

	private:

	};


	class Vector3
	{
	public:
		Vector3() = default;
		Vector3(const float x, const float y, const float z);

		static Vector3 Forward();
		static Vector3 Right();
		static Vector3 Up();

		Vector3& operator= (const Vector3& vec);

		float x;
		float y;
		float z;
	private:

	};


	class Vector4
	{
	public:
		Vector4() = default;
		Vector4(const float x, const float y, const float z, const float w);

		float x;
		float y;
		float z;
		float w;

	private:

	};
}

