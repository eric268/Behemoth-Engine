#pragma once

#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "Physics/Collision/CollisionData.h"
#include "Physics/Collision/NarrowCollision.h"
#include "Physics/Collision/ColliderHelper.h"
#include "Misc/Log.h"

#include <type_traits>
#include <tuple>


namespace Behemoth
{
	class NarrowCollisionSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& regsitry);

	private:

		template <typename T, typename U>
		// requires HasCollider<T,U>
		bool IsCollision(TransformComponent* transform1, TransformComponent* transform2, T* collider1, U* collider2, ContactData& data)
		{
			if (!collider1 || !collider2 || !(collider1->collisionType & collider2->collisionLayer))
			{
				return false;
			}

			if (!transform1 || !transform2)
			{
				LOGMESSAGE(Error, "Null transform found");
				return false;
			}

			SetCollider(transform1, collider1);
			SetCollider(transform2, collider2);

			return CheckCollision(collider1->collider, collider2->collider, data);
		}

		void GenerateCollisionData(ECS::Registry& registry, const ECS::EntityHandle& dynamicHandle, const ECS::EntityHandle& hitHandle, const ContactData& contactData);
	};
}