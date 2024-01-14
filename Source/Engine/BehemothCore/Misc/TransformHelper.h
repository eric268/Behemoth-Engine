#pragma once
#include "Math/MathCore.h"
#include "ECS/Entity.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class TransformHelper
	{
	public:
		TransformHelper() = delete;

		// delete this 
		static BMath::Matrix4x4f GetWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Matrix4x4f& localTransform);

		static BMath::Matrix4x4f GetTransformNoRotation(const BMath::Matrix4x4f& position, const BMath::Vector3& scale);

		static BMath::Matrix3x3f ExtractRotationMatrix(const  BMath::Matrix4x4f& transformMatrix);
		static BMath::Matrix3x3f ExtractRotationMatrix(const  BMath::Matrix4x4f& transformMatrix, BMath::Vector3 scale);
		static BMath::Vector3 ExtractScale(const  BMath::Matrix4x4f& transformMatrix);
		static BMath::Vector3 ExtractPosition(const  BMath::Matrix4x4f& transformMatrix);

		static void NotifyChildrenTransformChange(ECS::Registry& registry, ECS::EntityHandle handle);
	};
}