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

	struct ISystem
	{
		virtual ~ISystem() = default;
		virtual void Run(const float deltaTime, Registry& registry) = 0;
	};

	template<HasRunMethod T>
	struct System : public ISystem
	{
		System() = default;

		void Run(const float deltaTime, Registry& registry) override
		{
			instance.Run(deltaTime, registry);
		}

		T instance;
	};
}
