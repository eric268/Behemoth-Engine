#pragma once
#include "Math/MathCore.h"
#include "ECS/Entity.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct TransformComponent;

	class TransformHelper
	{
	public:
		TransformHelper() = delete;

		static void UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp);

		// delete this 
		static BMath::BMatrix4x4 GetWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::BMatrix4x4& localTransform);
		static BMath::BMatrix3x3 GetWorldRotation(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::BMatrix3x3& localTransform);

		static TransformComponent* GetParentTransformComp(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::BMatrix4x4 GetParentTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::BMatrix3x3 GetParentRotation(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::Vector3 GetParentScale(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::Vector3 GetParentPosition(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);


		static BMath::Vector3 GetWorldPosition(ECS::Registry& registry, const ECS::EntityHandle& entityHanldle, const BMath::Vector3& localPosition);
		static BMath::Vector3 GetWorldScale(ECS::Registry& registry, const ECS::EntityHandle& entityHanldle, const BMath::Vector3& localScale);

		static BMath::BMatrix4x4 GetTransformNoRotation(const BMath::BMatrix4x4& position, const BMath::Vector3& scale);
		static BMath::BMatrix4x4 RemoveScale(const BMath::BMatrix4x4& transform, const BMath::Vector3& scale);

		static BMath::BMatrix3x3 ExtractRotationMatrix(const  BMath::BMatrix4x4& transformMatrix);
		static BMath::BMatrix3x3 ExtractRotationMatrix(const  BMath::BMatrix4x4& transformMatrix, BMath::Vector3 scale);
		static BMath::Vector3 ExtractScale(const  BMath::BMatrix4x4& transformMatrix);
		static BMath::Vector3 ExtractPosition(const  BMath::BMatrix4x4& transformMatrix);

		static void NotifyChildrenTransformChange(ECS::Registry& registry, ECS::EntityHandle handle);
	};
}