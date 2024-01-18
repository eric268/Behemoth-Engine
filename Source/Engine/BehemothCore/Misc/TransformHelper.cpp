#include "pch.h"
#include "TransformHelper.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void TransformHelper::UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp)
	{
		if (transformComp->parentIsDirty)
		{
			TransformComponent* parentTransform = TransformHelper::GetParentTransformComp(registry, handle);
			if (parentTransform)
			{
				// Combine parent's world transform with child's local transform
				BMath::Matrix4x4f combinedTransform = TransformHelper::RemoveScale(parentTransform->worldTransform, parentTransform->worldScale) *
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
			}
		}
		else
		{
			transformComp->worldTransform = transformComp->localTransform;
			transformComp->worldPosition = transformComp->localPosition;
			transformComp->isDirty = true;
		}
	}

	BMath::Matrix4x4f TransformHelper::GetWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Matrix4x4f& localTransform)
	{
		BMath::Matrix4x4f worldTransform = localTransform;

		if (ChildComponent* childComp = registry.GetComponent<ChildComponent>(entityHandle))
		{
			TransformComponent* parentTransform = registry.GetComponent<TransformComponent>(childComp->parentHandle);
			if (parentTransform)
			{
				worldTransform =  parentTransform->worldTransform * localTransform;
			}
			else
			{
				LOGMESSAGE(MessageType::Error, "Parent transform not found for entity: " + registry.GetName(entityHandle));
			}
		}

		return worldTransform;
	}
	BMath::Matrix3x3f TransformHelper::GetWorldRotation(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Matrix3x3f& localRotation)
	{
		BMath::Matrix3x3f worldRotation = localRotation;

		if (ChildComponent* childComp = registry.GetComponent<ChildComponent>(entityHandle))
		{
			TransformComponent* parentTransform = registry.GetComponent<TransformComponent>(childComp->parentHandle);
			if (parentTransform)
			{
				BMath::Matrix3x3f parentRotation = ExtractRotationMatrix(parentTransform->worldTransform, parentTransform->worldScale);

				worldRotation = parentRotation * localRotation;
			}
			else
			{
				LOGMESSAGE(MessageType::Error, "Parent transform not found for entity: " + registry.GetName(entityHandle));
			}
		}
		return worldRotation;
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

	BMath::Matrix4x4f TransformHelper::GetParentTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
	{
		if (TransformComponent* transform = GetParentTransformComp(registry, entityHandle))
		{
			return transform->worldTransform;
		}
		return BMath::Matrix4x4f::Identity();
	}

	BMath::Matrix3x3f TransformHelper::GetParentRotation(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
	{
		return ExtractRotationMatrix(GetParentTransform(registry, entityHandle));
	}

	BMath::Vector3 TransformHelper::GetParentScale(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
	{
		if (TransformComponent* transform = GetParentTransformComp(registry, entityHandle))
		{
			return transform->worldScale;
		}
		return BMath::Vector3(1.0f);
	}

	BMath::Vector3 TransformHelper::GetParentPosition(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
	{
		if (TransformComponent* transform = GetParentTransformComp(registry, entityHandle))
		{
			return transform->worldPosition;
		}
		return BMath::Vector3(0.0f);
	}

	BMath::Vector3 TransformHelper::GetWorldPosition(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Vector3& localPosition)
	{
		BMath::Vector3 worldPosition = localPosition;

		if (ChildComponent* childComp = registry.GetComponent<ChildComponent>(entityHandle))
		{
			TransformComponent* parentTransform = registry.GetComponent<TransformComponent>(childComp->parentHandle);
			if (parentTransform)
			{
				worldPosition += parentTransform->worldPosition;
			}
			else
			{
				LOGMESSAGE(MessageType::Error, "Parent transform not found for entity: " + registry.GetName(entityHandle));
			}
		}

		return worldPosition;
	}

	BMath::Vector3 TransformHelper::GetWorldScale(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Vector3& localScale)
	{
		BMath::Vector3 worldScale = localScale;

		if (ChildComponent* childComp = registry.GetComponent<ChildComponent>(entityHandle))
		{
			TransformComponent* parentTransform = registry.GetComponent<TransformComponent>(childComp->parentHandle);
			if (parentTransform)
			{
				worldScale *= parentTransform->worldScale;
			}
			else
			{
				LOGMESSAGE(MessageType::Error, "Parent transform not found for entity: " + registry.GetName(entityHandle));
			}
		}

		return worldScale;
	}

	BMath::Matrix4x4f TransformHelper::GetTransformNoRotation(const BMath::Matrix4x4f& m, const BMath::Vector3& scale)
	{
		BMath::Matrix4x4f result = BMath::Matrix4x4f::Identity();

		result._11 = scale[0];
		result._22 = scale[1];
		result._33 = scale[2];

		result._41 = m._41;
		result._42 = m._42;
		result._43 = m._43;

		return result;
	}

	BMath::Matrix4x4f TransformHelper::RemoveScale(const BMath::Matrix4x4f& transform, const BMath::Vector3& scale)
	{
		BMath::Matrix4x4f m = BMath::Matrix4x4f::Identity();
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

	BMath::Matrix3x3f TransformHelper::ExtractRotationMatrix(const  BMath::Matrix4x4f& transformMatrix)
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

	BMath::Matrix3x3f TransformHelper::ExtractRotationMatrix(const  BMath::Matrix4x4f& transformMatrix, BMath::Vector3 scale)
	{
		BMath::Matrix3x3f m{};

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

	BMath::Vector3 TransformHelper::ExtractScale(const  BMath::Matrix4x4f& transformMatrix)
	{
		BMath::Vector3 scale{};

		for (int i = 0; i < 3; i++)
		{
			scale[i] = sqrt(transformMatrix.data[i][0] * transformMatrix.data[i][0] +
								transformMatrix.data[i][1] * transformMatrix.data[i][1] +
								transformMatrix.data[i][2] * transformMatrix.data[i][2]);
		}
		return scale;
	}

	BMath::Vector3 TransformHelper::ExtractPosition(const BMath::Matrix4x4f& transformMatrix)
	{
		return BMath::Vector3(transformMatrix._41, transformMatrix._42, transformMatrix._43);
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