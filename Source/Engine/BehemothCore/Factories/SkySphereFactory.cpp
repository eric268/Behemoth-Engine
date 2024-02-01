#include "pch.h"
#include "SkySphereFactory.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"

namespace Behemoth
{
    ECS::EntityHandle SkySphereFactory::CreateSkySphere(
		ECS::Registry& registry,
		const std::string& texturePath,
		const float sphereScale,
		const BMath::Vector2& uvScale)
    {
		ECS::EntityHandle skySphere = registry.CreateEntity("Sky Sphere");

		registry.AddComponent<Behemoth::TransformComponent>(skySphere);
		registry.AddComponent<Behemoth::ScalingComponent>(skySphere, BMath::Vector3(sphereScale));
		registry.AddComponent<Behemoth::RotationComponent>(skySphere, BMath::Quaternion(DEGREE_TO_RAD(180), BMath::Vector3(0, 0, 1)));

		if (SkySphereComponent* skySphereComp = registry.AddComponent<SkySphereComponent>(skySphere, texturePath, uvScale, true))
		{
			skySphereComp->mesh.materialData.specular = BMath::Vector3(0.0f);
			skySphereComp->mesh.materialData.diffuse = BMath::Vector3(0.0f);
		}
		else
		{
			LOGMESSAGE(Error, "Unable to add sky sphere component");
		}

		return skySphere;
    }
}
