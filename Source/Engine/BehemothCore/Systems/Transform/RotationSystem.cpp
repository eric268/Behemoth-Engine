#include "pch.h"
#include "RotationSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Core/Log.h""
#include "Misc/BColors.h"
#include "Misc/TransformHelper.h"
#include "Geometry/Primitives.h"

namespace Behemoth
{
	void RotationSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entityHandle, rotationComp, transformComp] : registry.Get<RotationComponent, TransformComponent>())
		{
			// TODO:
			// Find a better way to handle parent rotation changes so that this can be removed after a rotation is processed
			// Currently if a parent entity is rotated and this is removed when a rotation is processed a child may not have a rotation component
			// need to find better solution

// 			if (BMath::Quaternion::Equals(rotationComp->quat, BMath::Quaternion::Identity()) && rotationComp->isAdditive)
// 			{
// 				continue;
// 			}

			BMath::Matrix4x4 rotationMatrix = BMath::Quaternion::QuaternionToMatrix(rotationComp->quat);

			// Ensure local transform is updated first
			ApplyRotation(transformComp, rotationMatrix, rotationComp->isAdditive);
			TransformHelper::UpdateWorldTransform(registry, entityHandle, transformComp);
			TransformHelper::NotifyChildrenTransformChange(registry, entityHandle);
			transformComp->isDirty = true;

			transformComp->forwardVector = GetForwardVector(transformComp->localTransform);
			transformComp->rightVector = GetRightVector(transformComp->localTransform);
			transformComp->upVector = GetUpVector(transformComp->localTransform);

			rotationComp->isAdditive = true;
			rotationComp->quat = BMath::Quaternion::Identity();


			// If this entity has a camera component we need to update the view matrix as well after a rotation

			if (CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entityHandle))
			{
				cameraComponent->isDirty = true;
			}

			// If this entity has a mesh component then we need to update the mesh normals after a rotation
			if (MeshComponent* meshComp = registry.GetComponent<MeshComponent>(entityHandle))
			{
				RotateMeshNormals(meshComp, rotationMatrix);
			}

			// TODO:
			// Eventually would like to remove rotation components after processed
			// registry.RemoveComponent<RotationComponent>(entity);
		}
	}

	void RotationSystem::ApplyRotation(TransformComponent* transformComp, const BMath::Matrix4x4& rotationMatrix, bool isAdditive)
	{
		BMath::Matrix4x4 transform = (isAdditive) ?
			transformComp->localTransform :
			TransformHelper::GetTransformNoRotation(transformComp->localTransform, transformComp->localScale);

		BMath::Matrix4x4 rotatedTransformMatrix = rotationMatrix * transform;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				transformComp->localTransform.data[i][j] = rotatedTransformMatrix.data[i][j];
			}
		}
	}

	void RotationSystem::RotateMeshNormals(MeshComponent* meshComponent, const BMath::Matrix4x4& rotationMatrix)
	{
		for (int i = 0; i < meshComponent->mesh.meshPrimitives.size(); i++)
		{
			// This method is faster than recalculating the normal, but this rotation matrix does not take into account parent rotation
			// Need to check if getting the mesh normal from resource manager, then multiplying by world transform, then setting primitive normal is still faster
			// BMath::Vector3::RotateVector(meshComponent->mesh.meshPrimitives[i].normals[0], rotationMatrix, 0.0f);

			Primitive& primitive = meshComponent->mesh.meshPrimitives[i];
			primitive.normals[0] = BMath::Vector3(BMath::Vector4::Cross(primitive.vertices[1] - primitive.vertices[0], primitive.vertices[2] - primitive.vertices[0]));
			primitive.normals[0].Normalize();
		}
	}

	void RotationSystem::UpdateMeshNormalsFromParentRotation(TransformComponent* transformComp, MeshComponent* meshComponent)
	{
		BMath::Matrix3x3 extractedLocal = TransformHelper::ExtractRotationMatrix(transformComp->localTransform, transformComp->localScale);
		BMath::Matrix3x3 extractedWorld = TransformHelper::ExtractRotationMatrix(transformComp->worldTransform, transformComp->worldScale);

		BMath::Matrix3x3 inverseLocal = BMath::Matrix3x3::Inverse(extractedLocal);
		BMath::Matrix3x3 diffInRotation = extractedWorld * inverseLocal;

		for (int i = 0; i < meshComponent->mesh.meshPrimitives.size(); i++)
		{
			meshComponent->mesh.meshPrimitives[i].normals[0] = diffInRotation * meshComponent->mesh.meshPrimitives[i].normals[0];
		}
	}


	BMath::Vector3 RotationSystem::GetForwardVector(const BMath::Matrix4x4& transformMatrix)
	{
		return  BMath::Vector3(-transformMatrix._31, -transformMatrix._32, -transformMatrix._33).Normalize();
	}

	BMath::Vector3 RotationSystem::GetUpVector(const BMath::Matrix4x4& transformMatrix)
	{
		return  BMath::Vector3(transformMatrix._21, transformMatrix._22, transformMatrix._23).Normalize();
	}

	BMath::Vector3 RotationSystem::GetRightVector(const BMath::Matrix4x4& transformMatrix)
	{
		return BMath::Vector3(transformMatrix._11, transformMatrix._12, transformMatrix._13).Normalize();
	}
}
