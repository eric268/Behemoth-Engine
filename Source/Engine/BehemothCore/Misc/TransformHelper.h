#pragma once
#include "Math/MathCore.h"
#include "ECS/Entity.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class TransformComponent;

	class TransformHelper
	{
	public:
		TransformHelper() = delete;

		static void UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp);

		// delete this 
		static BMath::Matrix4x4f GetWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Matrix4x4f& localTransform);
		static BMath::Matrix3x3f GetWorldRotation(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Matrix3x3f& localTransform);

		static TransformComponent* GetParentTransformComp(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::Matrix4x4f GetParentTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::Matrix3x3f GetParentRotation(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::Vector3 GetParentScale(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::Vector3 GetParentPosition(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);


		static BMath::Vector3 GetWorldPosition(ECS::Registry& registry, const ECS::EntityHandle& entityHanldle, const BMath::Vector3& localPosition);
		static BMath::Vector3 GetWorldScale(ECS::Registry& registry, const ECS::EntityHandle& entityHanldle, const BMath::Vector3& localScale);

		static BMath::Matrix4x4f GetTransformNoRotation(const BMath::Matrix4x4f& position, const BMath::Vector3& scale);
		static BMath::Matrix4x4f RemoveScale(const BMath::Matrix4x4f& transform, const BMath::Vector3& scale);

		static BMath::Matrix3x3f ExtractRotationMatrix(const  BMath::Matrix4x4f& transformMatrix);
		static BMath::Matrix3x3f ExtractRotationMatrix(const  BMath::Matrix4x4f& transformMatrix, BMath::Vector3 scale);
		static BMath::Vector3 ExtractScale(const  BMath::Matrix4x4f& transformMatrix);
		static BMath::Vector3 ExtractPosition(const  BMath::Matrix4x4f& transformMatrix);

		static void NotifyChildrenTransformChange(ECS::Registry& registry, ECS::EntityHandle handle);
	};
}