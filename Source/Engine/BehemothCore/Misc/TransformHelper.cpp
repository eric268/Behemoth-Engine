#include "pch.h"
#include "TransformHelper.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"
#include "Core/Log.h"

namespace Behemoth
{
	void TransformHelper::UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp)
	{
		if (TransformComponent* parentTransformComp = TransformHelper::GetParentTransformComp(registry, handle))
		{
			// Combine parent's world transform with child's local transform
			BMath::Matrix4x4 combinedTransform = TransformHelper::RemoveScale(parentTransformComp->worldTransform) *
				TransformHelper::RemoveScale(transformComp->localTransform);

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					transformComp->worldTransform.data[i][j] = combinedTransform.data[i][j] * transformComp->worldScale[i];
				}
			}

			transformComp->worldPosition = BMath::Vector3(combinedTransform._41, combinedTransform._42, combinedTransform._43);
			transformComp->worldTransform._41 = transformComp->worldPosition.x;
			transformComp->worldTransform._42 = transformComp->worldPosition.y;
			transformComp->worldTransform._43 = transformComp->worldPosition.z;
			transformComp->worldScale = parentTransformComp->worldScale * transformComp->localScale;
			transformComp->parentIsDirty = false;
		}
		else
		{
			transformComp->worldTransform = transformComp->localTransform;
			transformComp->worldPosition = transformComp->localPosition;
			transformComp->worldScale = transformComp->localScale;
			transformComp->isDirty = true;
		}
	}

	TransformComponent* TransformHelper::GetParentTransformComp(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
	{
		if (ChildComponent* childComp = registry.GetComponent<ChildComponent>(entityHandle))
		{
			TransformComponent* parentTransformComp = registry.GetComponent<TransformComponent>(childComp->parentHandle);
			if (parentTransformComp)
			{
				return parentTransformComp;
			}
			else
			{
				LOGMESSAGE(MessageType::Error, "Parent transform not found for entity: " + registry.GetName(entityHandle));
			}
		}
		return nullptr;
	}

	BMath::Matrix4x4 TransformHelper::GetParentTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
	{
		if (TransformComponent* transformComp = GetParentTransformComp(registry, entityHandle))
		{
			return transformComp->worldTransform;
		}
		return BMath::Matrix4x4::Identity();
	}

	BMath::Matrix4x4 TransformHelper::GetTransformNoRotation(const BMath::Matrix4x4& m, const BMath::Vector3& scale)
	{
		BMath::Matrix4x4 result = BMath::Matrix4x4::Identity();

		result._11 = scale[0];
		result._22 = scale[1];
		result._33 = scale[2];

		result._41 = m._41;
		result._42 = m._42;
		result._43 = m._43;

		return result;
	}

	BMath::Matrix3x3 TransformHelper::ExtractRotationMatrix(const  BMath::Matrix4x4& transformMatrix, BMath::Vector3 scale)
	{
		BMath::Matrix3x3 m{};

		for (int i = 0; i < 3; i++)
		{
			if (scale[i] == 0.0f)
			{
				scale[i] = EPSILON;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m.data[i][j] = transformMatrix.data[i][j] / scale[j];
			}
		}
		return m;
	}

	BMath::Matrix4x4 TransformHelper::RemoveScale(const BMath::Matrix4x4& transform)
	{
		BMath::Matrix4x4 m = transform;

		for (int i = 0; i < 3; i++)
		{
			float length = 0.0f;
			for (int j = 0; j < 3; j++)
			{
				length += m.data[j][i] * m.data[j][i];
			}
			length = sqrt(length);

			if (length != 0)
			{
				for (int j = 0; j < 3; j++)
				{
					m.data[j][i] /= length;
				}
			}
		}

		return m;
	}

	void TransformHelper::NotifyChildrenTransformChange(ECS::Registry& registry, ECS::EntityHandle handle)
	{

		if (const auto& parentComp = registry.GetComponent<ParentComponent>(handle))
		{
			for (int i = parentComp->childHandles.size() - 1; i >= 0; i--)
			{
				// First check to ensure child still exists, if not then remove child from parent component container
				ChildComponent* childComp = registry.GetComponent<ChildComponent>(parentComp->childHandles[i]);
				if (!childComp)
				{
					parentComp->childHandles.erase(parentComp->childHandles.begin() + i);
					continue;
				}

				if (TransformComponent* childTransformComp = registry.GetComponent<TransformComponent>(parentComp->childHandles[i]))
				{
					childTransformComp->parentIsDirty = true;
				}
			}
		}
	}
}