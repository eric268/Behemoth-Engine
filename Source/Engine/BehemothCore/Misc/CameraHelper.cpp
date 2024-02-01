#include "pch.h"
#include "CameraHelper.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Math/MathCore.h"
#include "Core/Log.h"
#include "Components/Components.h"

namespace Behemoth
{
	BMath::Matrix4x4 CameraHelper::LookAt(const BMath::Vector3& eye, const BMath::Vector3& target, const BMath::Vector3& up)
	{
		const BMath::Vector3 zAxis = BMath::Vector3::Normalize(eye - target);
		const BMath::Vector3 xAxis = BMath::Vector3::Normalize(BMath::Vector3::Cross(up, zAxis));
		const BMath::Vector3 yAxis = BMath::Vector3::Cross(zAxis, xAxis);
		
		const float tx = -BMath::Vector3::Dot(xAxis, eye);
		const float ty = -BMath::Vector3::Dot(yAxis, eye);
		const float tz = -BMath::Vector3::Dot(zAxis, eye);

		BMath::Matrix4x4 viewMatrix =
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
		for (const auto& [entityHandle, cameraComp] : registry.Get<CameraComponent>())
		{
			if (cameraComp->isMain)
			{
				return cameraComp;
			}
		}
		LOGMESSAGE(MessageType::Error, "Unable to find main camera");
		return nullptr;
	}

	TransformComponent* CameraHelper::GetMainCameraTransform(ECS::Registry& registry)
	{
		for (const auto& [
			entityHandle, 
				cameraComp,
				transformComp] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (cameraComp->isMain)
			{
				return transformComp;
			}
		}
		LOGMESSAGE(MessageType::Error, "Unable to find main camera");
		return nullptr;
	}

	BMath::Vector3 CameraHelper::GetMainCameraPosition(ECS::Registry& registry)
	{
		for (const auto& [
			entityHandle, 
				cameraComp, 
				transformComp] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (cameraComp->isMain)
			{
				return transformComp->worldPosition;
			}
		}
		LOGMESSAGE(MessageType::Error, "Unable to find main camera");
		return BMath::Vector3::Zero();
	}

	const ECS::EntityHandle& CameraHelper::GetMainCameraEntity(ECS::Registry& registry)
	{
		for (const auto& [
			entityHandle, 
				cameraComp,
				transformComp] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (cameraComp->isMain)
			{
				return entityHandle;
			}
		}
		LOGMESSAGE(MessageType::Error, "Unable to find main camera");
		return registry.CreateNullEntity();
	}

	BMath::Vector3 CameraHelper::GetForwardVector(ECS::Registry& registry)
	{
		CameraComponent* cameraComp = GetMainCamera((registry));
		if (!cameraComp)
		{
			LOGMESSAGE(Error, "Unable to find main camera component");
			return BMath::Vector3::Zero();
		}
		return BMath::Vector3(cameraComp->viewMatrix._31, cameraComp->viewMatrix._32, cameraComp->viewMatrix._33);
	}
}