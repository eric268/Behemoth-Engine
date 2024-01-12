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
				if (entity == entity2)
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
					LOG_MESSAGE(MessageType::Warning, "Colliding " + registry.GetName(entity));
					BMath::Vector3 offset = contactData.collisionNormal * contactData.penetrationDepth;
					// LOG_MESSAGE(MessageType::General, "Collision - Offset: X" + std::to_string(offset.x) + " Y: " + std::to_string(offset.y) + " Z: " + std::to_string(offset.z));
					registry.AddComponent<MoveComponent>(entity, offset);
				}
			}
		}
	}
}