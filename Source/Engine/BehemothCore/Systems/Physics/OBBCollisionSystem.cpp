#include "pch.h"
#include "OBBCollisionSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Collision/NarrowCollision.h"
#include "Physics/Collision/BroadCollision.h"
#include "Physics/Collision/CollisionData.h"

namespace Behemoth
{
	void OBBCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<OBBColliderComponent, TransformComponent>();

		for (const auto& [entity, OBBComp, transformComp] : components)
		{
			for (const auto& [entity2, OBBComp2, transformComp2] : components)
			{
				if (entity == entity2 || registry.GetComponent<StaticComponent>(entity))
				{
					continue;
				}

				OBBComp->collider.position = transformComp->worldPosition;
				OBBComp2->collider.position = transformComp2->worldPosition;

				OBBComp->collider.extents = OBBComp->extents * transformComp->worldScale;
				OBBComp2->collider.extents = OBBComp2->extents * transformComp2->worldScale;

				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						OBBComp->collider.orientation[i][j] = transformComp->worldTransform.data[i][j] / transformComp->worldScale[i];
						OBBComp2->collider.orientation[i][j] = transformComp2->worldTransform.data[i][j] / transformComp2->worldScale[i];
					}
				}

				ContactData contactData{};
				if ((OBBComp->collisionType & OBBComp2->collisionLayer) && NarrowOBBOBBCollision(OBBComp->collider, OBBComp2->collider, contactData))
				{
					NarrowOBBOBBCollision(OBBComp->collider, OBBComp2->collider, contactData);
					LOGMESSAGE(Warning, "Colliding " + registry.GetName(entity));
					BMath::Vector3 offset = contactData.collisionNormal * contactData.penetrationDepth;
					LOGMESSAGE(General, "Collision - Offset: X" + std::to_string(offset.x) + " Y: " + std::to_string(offset.y) + " Z: " + std::to_string(offset.z));
					// registry.AddComponent<MoveComponent>(entity, offset);
					 offset *= 1.1f;

// 					transformComp->localTransform._41 += offset.x;
// 					transformComp->localTransform._42 += offset.y;
// 					transformComp->localTransform._43 += offset.z;

					transformComp->worldTransform._41 += offset.x;
					transformComp->worldTransform._42 += offset.y;
					transformComp->worldTransform._43 += offset.z;

					transformComp->worldPosition += offset;
					// transformComp->localPosition += offset;

					transformComp->isDirty = true;

					VelocityComponent* vel = registry.GetComponent<VelocityComponent>(entity);
					vel->velocity.y += 9.81 * deltaTime;
				}
			}
		}
	}
}