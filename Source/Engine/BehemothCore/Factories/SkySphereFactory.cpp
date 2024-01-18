#include "pch.h"
#include "SkySphereFactory.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"

namespace Behemoth
{
    ECS::EntityHandle SkySphereFactory::CreateSkySphere(ECS::Registry& registry,const std::string& texturePath, const BMath::Vector2& uvScale)
    {
		ECS::EntityHandle skySphere = registry.CreateEntity("Sky Sphere");
		registry.AddComponent<Behemoth::TransformComponent>(skySphere);
		auto sphere = registry.AddComponent<Behemoth::SkySphereComponent>(skySphere, texturePath, uvScale, true);
		registry.AddComponent<Behemoth::ScalingComponent>(skySphere, BMath::Vector3(999.0f));
		registry.AddComponent<Behemoth::RotationComponent>(skySphere, BMath::Quaternion(DEGREE_TO_RAD(180), BMath::Vector3(0, 0, 1)));
		sphere->mesh.materialData.specular = BMath::Vector3(0.0f);
		sphere->mesh.materialData.diffuse = BMath::Vector3(0.0f);
		return skySphere;
    }
}
