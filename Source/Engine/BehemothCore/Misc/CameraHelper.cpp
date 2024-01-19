#include "pch.h"
#include "CameraHelper.h"
#include "Math/Vector.h"
#include "Core/Log.h"
#include "ECS/Registry.h"
#include "Components/Components.h"

#include <cassert>

namespace Behemoth
{
	BMath::BMatrix4x4 CameraHelper::LookAt(const BMath::Vector3& eye, const BMath::Vector3& target, const BMath::Vector3& up)
	{
		const BMath::Vector3 zAxis = BMath::Vector3::Normalize(eye - target);
		const BMath::Vector3 xAxis = BMath::Vector3::Normalize(BMath::Vector3::Cross(up, zAxis));
		const BMath::Vector3 yAxis = BMath::Vector3::Cross(zAxis, xAxis);
		
		const float tx = -BMath::Vector3::Dot(xAxis, eye);
		const float ty = -BMath::Vector3::Dot(yAxis, eye);
		const float tz = -BMath::Vector3::Dot(zAxis, eye);

		BMath::BMatrix4x4 viewMatrix =
		{
			{ xAxis.x, xAxis.y, xAxis.z, tx},
			{ yAxis.x, yAxis.y, yAxis.z, ty},
			{ zAxis.x, zAxis.y, zAxis.z, tz},
			{ 0.0f,	   0.0f,    0.0f,    1.0f}
		};

		return viewMatrix;
	}

	CameraComponent* CameraHelper::GetMainCamera(ECS::Registry& registry)
	{
		for (const auto& [entity, camera] : registry.Get<CameraComponent>())
		{
			if (camera->isMain)
				return camera;
		}
		LOGMESSAGE(MessageType::Error, "Unable to find main camera");
		assert(nullptr);

		return nullptr;
	}

	TransformComponent* CameraHelper::GetMainCameraTransform(ECS::Registry& registry)
	{
		for (const auto& [entity, camera, transformComp] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (camera->isMain)
				return transformComp;
		}
		LOGMESSAGE(MessageType::Error, "Unable to find main camera");
		assert(nullptr);

		return nullptr;
	}

	BMath::Vector3 CameraHelper::GetMainCameraPostition(ECS::Registry& registry)
	{
		for (const auto& [entity, camera, transform] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (camera->isMain)
				return transform->worldPosition;
		}
		LOGMESSAGE(MessageType::Error, "Unable to find main camera");
		assert(nullptr);

		return{};
	}

	ECS::Entity CameraHelper::GetMainCameraEntity(ECS::Registry& registry)
	{
		for (const auto& [entity, camera, transform] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (camera->isMain)
				return entity;
		}
		LOGMESSAGE(MessageType::Error, "Unable to find main camera");
		assert(nullptr);

		return registry.CreateNullEntity();
	}

}