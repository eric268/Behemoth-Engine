#pragma once
#include "Registry.h"

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
			instance.Run(deltaTime, registry);
		}

		T instance;
	private:
	};
}
