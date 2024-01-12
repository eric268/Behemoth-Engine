#include "pch.h"
#include "SphereCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision/NarrowCollision.h"
#include "Physics/Collision/CollisionData.h"

namespace Behemoth
{
	void SphereCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<SphereColliderComponent, TransformComponent>();

		for (const auto& [entity, sphereColliderComp, transformComp] : components)
		{
			for (const auto& [entity2, sphereColliderComp2, transformComp2] : components)
			{
				if (entity == entity2)
				{
					continue;
				}

				sphereColliderComp->collider.position = transformComp->worldPosition;
				sphereColliderComp->collider.radius = sphereColliderComp->radius * std::max(transformComp->worldScale[0], std::max(transformComp->worldScale[1], transformComp->worldScale[2]));

				sphereColliderComp2->collider.position = transformComp2->worldPosition;
				sphereColliderComp2->collider.radius = sphereColliderComp2->radius * std::max(transformComp2->worldScale[0], std::max(transformComp2->worldScale[1], transformComp2->worldScale[2]));

				ContactData contactData{};
				if ((sphereColliderComp->collisionType & sphereColliderComp2->collisionLayer) && NarrowSphereSphereCollision(sphereColliderComp->collider, sphereColliderComp2->collider, contactData))
				{
					BMath::Vector3 offset = contactData.collisionNormal * contactData.penetrationDepth;
					LOGMESSAGE(MessageType::General, "Collision - Offset: X" + std::to_string(offset.x) + " Y: " + std::to_string(offset.y) + " Z: " + std::to_string(offset.z));
					BMath::Vector3 newLocation = BMath::Vector3(transformComp->localTransform._41 + offset.x, transformComp->localTransform._42 + offset.y, transformComp->localTransform._43 + offset.z);
					registry.AddComponent<MoveComponent>(entity, offset);

					transformComp->isDirty = true;
				}
			}
		}
	}
}