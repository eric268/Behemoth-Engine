#pragma once
#include "Registry.h"

namespace ECS
{
	template<typename T>
	concept HasRunMethod = requires(T t, Registry r)
	{
		{ t.Run(r) } -> std::same_as<void>;
	};

	class ISystem
	{
	public:
		virtual ~ISystem() = default;
		virtual void Run(Registry& registry) {}
	};

	template<HasRunMethod T>
	class System : public ISystem
	{
	public:
		System() = default;

		void Run(Registry& registry) override
		{
			instance.Run(registry);
		}

		T instance;
	private:
	};
}
