#pragma once

#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision/CollisionData.h"
#include "Physics/Collision/NarrowCollision.h"

#include <type_traits>
#include <tuple>


namespace Behemoth
{
// 	template <typename T, typename U>
// 	concept HasCollider = requires (T t, U u)
// 	{
// 		{ t.collider } -> std::same_as<decltype(t.collider)>;
// 		{ u.collider } -> std::same_as<decltype(u.collider)>;
// 	};


	class NarrowCollisionSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& regsitry);

	private:

		template <typename ...Colliders>
		std::tuple<Colliders*...> GetColliders(ECS::Registry& registry, const ECS::EntityHandle& handle, CollidersContainer<Colliders ...>)
		{
			return registry.GetMultipleComponents<Colliders...>(handle);
		}

		template <typename T, typename U>
		// requires HasCollider<T,U>
		bool GenerateCollisionData(T* collider1, U* collider2, ContactData& data)
		{
			if (!collider1 || !collider2)
			{
				return false;
			}
			
			return CheckCollision(collider1->collider, collider2->collider, data);
		}

// 		template <typename T, typename = void>
// 		struct has_collider : std::false_type {};
// 
// 		template <typename T>
// 		struct has_collider<T, decltype(std::declval<T>().collider)> : std::true_type {};
// 
// 		template <typename T, typename U, typename V = void>
// 		struct DataGenerator : public std::false_type {};
// 
// 		template <typename T, typename U>
// 		struct DataGenerator<T, U, std::enable_if<has_collider<T>::value && has_collider<U>::value>> : public std::true_type {};

// 		template <typename T, typename U>
// 		typename std::enable_if<!DataGenerator<T, U>::value, bool>::type
// 		GenerateCollisionData(T*, U*, ContactData&) 
// 		{
// 			return false;
// 		}
	};
}