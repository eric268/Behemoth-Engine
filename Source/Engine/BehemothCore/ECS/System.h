#pragma once
#include "Registry.h"
#include "Core/Stopwatch.h"

#include <iostream>

namespace ECS
{
	template<typename T>
	concept HasRunMethod = requires(T t, float dt, Registry r)
	{
		{ t.Run(dt, r) } -> std::same_as<void>;
	};

	class ISystem
	{
	public:
		virtual ~ISystem() = default;
		virtual void Run(const float deltaTime, Registry& registry) = 0;
	};

	template<HasRunMethod T>
	class System : public ISystem
	{
	public:
		System() = default;

		void Run(const float deltaTime, Registry& registry) override
		{
			{
				// Behemoth::Stopwatch sw;
				// std::cout << typeid(T).name() << std::endl;
				instance.Run(deltaTime, registry);
			}
		}

		T instance;
	private:
	};
}
