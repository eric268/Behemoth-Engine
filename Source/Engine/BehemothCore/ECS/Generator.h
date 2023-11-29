#pragma once

namespace ECS
{
	class Generator
	{
	public:
		template <typename T>
		static int Value()
		{
			static int value = Initalize();
			return value;
		}

	private:
		static int Initalize()
		{
			static int value = 0;
			return value++;
		}
	};
}

