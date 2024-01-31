#pragma once

namespace ECS
{
	class Generator
	{
	public:
		template <typename T>
		static int Value()
		{
			static int value = Initialize();
			return value;
		}

	private:
		static int Initialize()
		{
			static int value = 0;
			return value++;
		}
	};
}

