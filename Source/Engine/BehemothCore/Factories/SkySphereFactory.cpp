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
		ECS::EntityHandle skySphereHandle = registry.CreateEntity("Sky Sphere");

		registry.AddComponent<Behemoth::TransformComponent>(skySphereHandle);
		registry.AddComponent<Behemoth::ScalingComponent>(skySphereHandle, BMath::Vector3(sphereScale));
		registry.AddComponent<Behemoth::RotationComponent>(skySphereHandle, BMath::Quaternion(DEGREE_TO_RAD(180.0f), BMath::Vector3(0.0f, 0.0f, 1.0f)));

		if (SkySphereComponent* skySphereComp = registry.AddComponent<SkySphereComponent>(skySphereHandle, texturePath, uvScale, true))
		{
			skySphereComp->mesh.materialData.specular = BMath::Vector3(0.0f);
			skySphereComp->mesh.materialData.diffuse = BMath::Vector3(0.0f);
		}
		else
		{
			LOGMESSAGE(Error, "Unable to add sky sphere component");
		}

		return skySphereHandle;
    }
}
