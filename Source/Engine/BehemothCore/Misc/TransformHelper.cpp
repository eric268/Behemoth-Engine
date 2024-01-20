#include "pch.h"
#include "TransformHelper.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Core/Log.h"

namespace Behemoth
{
	void TransformHelper::UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp)
	{
		TransformComponent* parentTransform = TransformHelper::GetParentTransformComp(registry, handle);
		if (parentTransform)
		{
			// if (transformComp->parentIsDirty)
			{
				// Combine parent's world transform with child's local transform
				BMath::BMatrix4x4 combinedTransform = TransformHelper::RemoveScale(parentTransform->worldTransform, parentTransform->worldScale) *
					TransformHelper::RemoveScale(transformComp->localTransform, transformComp->localScale);

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
				transformComp->worldScale = parentTransform->worldScale * transformComp->localScale;
				transformComp->parentIsDirty = false;
			}
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
			TransformComponent* parentTransform = registry.GetComponent<TransformComponent>(childComp->parentHandle);
			if (parentTransform)
			{
				return parentTransform;
			}
			else
			{
				LOGMESSAGE(MessageType::Error, "Parent transform not found for entity: " + registry.GetName(entityHandle));
			}
		}
		return nullptr;
	}

	BMath::BMatrix4x4 TransformHelper::GetParentTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
	{
		if (TransformComponent* transform = GetParentTransformComp(registry, entityHandle))
		{
			return transform->worldTransform;
		}
		return BMath::BMatrix4x4::Identity();
	}

	BMath::BMatrix4x4 TransformHelper::GetTransformNoRotation(const BMath::BMatrix4x4& m, const BMath::Vector3& scale)
	{
		BMath::BMatrix4x4 result = BMath::BMatrix4x4::Identity();

		result._11 = scale[0];
		result._22 = scale[1];
		result._33 = scale[2];

		result._41 = m._41;
		result._42 = m._42;
		result._43 = m._43;

		return result;
	}

	BMath::BMatrix4x4 TransformHelper::RemoveScale(const BMath::BMatrix4x4& transform, const BMath::Vector3& scale)
	{
		BMath::BMatrix4x4 m = BMath::BMatrix4x4::Identity();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m.data[i][j] = transform.data[i][j] / scale[i];
			}
		}

		m._41 = transform._41;
		m._42 = transform._42;
		m._43 = transform._43;
		
		return m;
	}

	BMath::BMatrix3x3 TransformHelper::ExtractRotationMatrix(const  BMath::BMatrix4x4& transformMatrix)
	{
		BMath::Matrix3x3 m{};
		for (int col = 0; col < 3; col++)
		{
			float length = sqrt(transformMatrix.data[col][0] * transformMatrix.data[col][0] +
				transformMatrix.data[col][1] * transformMatrix.data[col][1] +
				transformMatrix.data[col][2] * transformMatrix.data[col][2]);

			if (length == 0.0f)
			{
				length = EPSILON;
			}

			for (int row = 0; row < 3; row++)
			{
				m.data[col][row] = transformMatrix.data[col][row] / length;
			}
		}
		return m;
	}

	BMath::BMatrix3x3 TransformHelper::ExtractRotationMatrix(const  BMath::BMatrix4x4& transformMatrix, BMath::Vector3 scale)
	{
		BMath::BMatrix3x3 m{};

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (scale[j] == 0.0f)
				{
					scale[j] = EPSILON;
				}

				m.data[i][j] = transformMatrix.data[i][j] / scale[i];
			}
		}
		return m;
	}

	void TransformHelper::NotifyChildrenTransformChange(ECS::Registry& registry, ECS::EntityHandle handle)
	{

		if (auto parentComp = registry.GetComponent<ParentComponent>(handle))
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