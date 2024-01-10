#include "pch.h"
#include "TransformHelper.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Misc/Log.h"

namespace Behemoth
{
	BMath::Matrix4x4 TransformHelper::GetWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, const BMath::Matrix4x4& localTransform)
	{
		BMath::Matrix4x4 worldTransform = localTransform;

		if (ChildComponent* childComp = registry.GetComponent<ChildComponent>(entityHandle))
		{
			TransformComponent* parentTransform = registry.GetComponent<TransformComponent>(childComp->parentHandle);
			if (parentTransform)
			{
				worldTransform =  parentTransform->worldTransform * localTransform;
			}
			else
			{
				LOG_MESSAGE(MessageType::Error, "Parent transform not found for entity: " + registry.GetName(entityHandle));
			}
		}

		return worldTransform;
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

	BMath::Matrix3x3 TransformHelper::ExtractRotationMatrix(const  BMath::Matrix4x4& transformMatrix)
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

	BMath::Matrix3x3 TransformHelper::ExtractRotationMatrix(const  BMath::Matrix4x4& transformMatrix, BMath::Vector3 scale)
	{
		BMath::Matrix3x3 m{};

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

	BMath::Vector3 TransformHelper::ExtractScale(const  BMath::Matrix4x4& transformMatrix)
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

	BMath::Vector3 TransformHelper::ExtractPosition(const BMath::Matrix4x4& transformMatrix)
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