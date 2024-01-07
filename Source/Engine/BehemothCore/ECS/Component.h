#pragma once

#include "Generator.h"
#include "Math/MathCore.h"

namespace ECS
{
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() {}
	}; 

	template<typename T, typename = void>
	struct CanCombineComponent : std::false_type {};

	template<typename T>
	struct CanCombineComponent<T, decltype(std::declval<T>() + std::declval<T>(), void())> : std::true_type {};
}