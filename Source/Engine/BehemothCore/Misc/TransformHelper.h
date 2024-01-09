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
		static BMath::Matrix4x4 GetWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Matrix4x4& localTransform);


		static BMath::Matrix3x3 ExtractRotationMatrix(const  BMath::Matrix4x4& transformMatrix);
		static BMath::Matrix3x3 ExtractRotationMatrix(const  BMath::Matrix4x4& transformMatrix, BMath::Vector3 scale);
		static BMath::Vector3 ExtractScale(const  BMath::Matrix4x4& transformMatrix);
		static BMath::Vector3 ExtractPosition(const  BMath::Matrix4x4& transformMatrix);

		static void NotifyChildrenTransformChange(ECS::Registry& registry, ECS::EntityHandle handle);
	};
}