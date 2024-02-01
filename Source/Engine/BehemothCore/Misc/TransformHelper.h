#pragma once

#include "Math/MathCore.h"

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	struct TransformComponent;

	class TransformHelper
	{
	public:
		TransformHelper() = delete;

		static void UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp);

		// Functions to assist with parent - child transform updates
		static void NotifyChildrenTransformChange(ECS::Registry& registry, ECS::EntityHandle handle);

		static TransformComponent* GetParentTransformComp(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);
		static BMath::Matrix4x4 GetParentTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle);

		static BMath::Matrix4x4 RemoveScale(const BMath::Matrix4x4& transform);

		static BMath::Matrix4x4 GetTransformNoRotation(const BMath::Matrix4x4& position, const BMath::Vector3& scale);
		static BMath::Matrix3x3 ExtractRotationMatrix(const  BMath::Matrix4x4& transformMatrix, BMath::Vector3 scale);

	};
}