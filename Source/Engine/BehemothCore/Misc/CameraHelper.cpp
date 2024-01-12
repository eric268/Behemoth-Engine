#include "pch.h"
#include "CameraHelper.h"
#include "Math/Vector.h"
#include "Log.h"
#include "ECS/Registry.h"
#include "Components/Components.h"

#include <cassert>

using namespace BMath;

namespace Behemoth
{
	BMath::Matrix4x4 CameraHelper::LookAt(const BMath::Vector3& eye, const BMath::Vector3& target, const BMath::Vector3& up)
	{
		const Vector3 zAxis = Vector3::Normalize(eye - target);
		const Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
		const Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

		const float tx = -Vector3::Dot(xAxis, eye);
		const float ty = -Vector3::Dot(yAxis, eye);
		const float tz = -Vector3::Dot(zAxis, eye);

		Matrix4x4 viewMatrix =
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