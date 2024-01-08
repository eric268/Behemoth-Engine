#include "pch.h"
#include "RotationSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Misc/Log.h""
#include "Misc/BColors.h"

#include "Geometry/Primitives.h"

namespace Behemoth
{
	void RotationSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<RotationComponent, TransformComponent>();

		for (const auto& [entity, rotationComp, transformComp] : components)
		{
//  			if (rotationComp->quat == BMath::Quaternion::Identity())
//  			{
//  				continue;
//  			}

			BMath::Matrix4x4 rotationMatrix = BMath::Quaternion::QuaternionToMatrix(rotationComp->quat);
			rotationComp->quat = BMath::Quaternion::Identity();
			RotateTransformMatrix(transformComp, rotationMatrix);

			// If this entity has a camera component we need to update the view matrix as well after a rotation
			CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity);
			if (cameraComponent)
			{
				cameraComponent->isDirty = true;
			}

			// If this entity has a mesh component then we need to update the mesh normals after a rotation
			MeshComponent* meshComp = registry.GetComponent<MeshComponent>(entity);
			if (meshComp)
			{
				RotateMeshNormals(meshComp, rotationMatrix);
			}
		}
	}

	void RotationSystem::RotateTransformMatrix(TransformComponent* transformComponent, const BMath::Matrix4x4& rotationMatrix)
	{
		BMath::Matrix4x4 rotatedTransformMatrix = rotationMatrix * transformComponent->transformMatrix;
		for (int col = 0; col < 3; col++)
		{
			for (int row = 0; row < 3; row++)
			{
				transformComponent->transformMatrix.data[col][row] = rotatedTransformMatrix.data[col][row];
			}
		}
		transformComponent->forwardVector = GetForwardVector(transformComponent->transformMatrix);
		transformComponent->rightVector = GetRightVector(transformComponent->transformMatrix);
		transformComponent->upVector = BMath::Vector3::Up();
		transformComponent->isDirty = true;
	}

	void RotationSystem::RotateMeshNormals(MeshComponent* meshComponent, const BMath::Matrix4x4& rotationMatrix)
	{
		for (int i = 0; i < meshComponent->mesh.meshPrimitives.size(); i++)
		{
			BMath::Vector3::RotateVector(meshComponent->mesh.meshPrimitives[i].normals[0], rotationMatrix, 0.0f);
		}
	}

	BMath::Vector3 RotationSystem::GetForwardVector(const BMath::Matrix4x4& transformMatrix)
	{
		return  BMath::Vector3(-transformMatrix._13, -transformMatrix._23, -transformMatrix._33).Normalize();
	}
	BMath::Vector3 RotationSystem::GetRightVector(const BMath::Matrix4x4& transformMatrix)
	{
		return BMath::Vector3(transformMatrix._11, transformMatrix._21, transformMatrix._31).Normalize();
	}
}
