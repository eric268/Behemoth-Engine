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
		auto components = registry.Get<RotationComponent, TransformComponent>();

		for (const auto& [entity, rotationComp, transformComp] : components)
		{
  			if (BMath::Quaternion::Equals(rotationComp->quat, BMath::Quaternion::Identity()) && !transformComp->parentIsDirty)
  			{
  				continue;
	  		}

			BMath::BMatrix4x4 rotationMatrix = BMath::Quaternion::QuaternionToMatrix(rotationComp->quat);
			rotationComp->quat = BMath::Quaternion::Identity();

			// Ensure local transform is updated first
			ApplyRotation(transformComp->localTransform, rotationMatrix);
			TransformHelper::UpdateWorldTransform(registry, entity, transformComp);
			TransformHelper::NotifyChildrenTransformChange(registry, entity);

			transformComp->isDirty = true;

			// If this entity has a camera component we need to update the view matrix as well after a rotation
			
			if (CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity))
			{
				cameraComponent->isDirty = true;
			}

			// If this entity has a mesh component then we need to update the mesh normals after a rotation
			if (MeshComponent*  meshComp = registry.GetComponent<MeshComponent>(entity))
			{
				RotateMeshNormals(meshComp, rotationMatrix);
			}
		}
	}

	void RotationSystem::ApplyRotation(BMath::BMatrix4x4& transform, const BMath::BMatrix4x4& rotationMatrix)
	{
		BMath::BMatrix4x4 rotatedTransformMatrix = rotationMatrix * transform;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++) 
			{
				transform.data[i][j] = rotatedTransformMatrix.data[i][j];
			}
		}
	}

	void RotationSystem::RotateMeshNormals(MeshComponent* meshComponent, const BMath::BMatrix4x4& rotationMatrix)
	{
		for (int i = 0; i < meshComponent->mesh.meshPrimitives.size(); i++)
		{
			// This method is faster than recalulating the normal, but this rotation matrix does not take into account parent rotation
			// Need to check if gettings the mesh normal from resource manager, then multiplying by world transform, then setting primitive normal is still faster
			// BMath::Vector3::RotateVector(meshComponent->mesh.meshPrimitives[i].normals[0], rotationMatrix, 0.0f);

			Primitive& primitive = meshComponent->mesh.meshPrimitives[i];
			primitive.normals[0] = BMath::Vector3(BMath::Vector4::Cross(primitive.verticies[1] - primitive.verticies[0], primitive.verticies[2] - primitive.verticies[0]));
			primitive.normals[0].Normalize();
		}
	}

	void RotationSystem::UpdateMeshNormalsFromParentRotation(TransformComponent* transformComp, MeshComponent* meshComponent)
	{
		BMath::BMatrix3x3 extractedLocal = TransformHelper::ExtractRotationMatrix(transformComp->localTransform);
		BMath::BMatrix3x3 extractedWorld = TransformHelper::ExtractRotationMatrix(transformComp->worldTransform);

		BMath::BMatrix3x3 inverseLocal = BMath::BMatrix3x3::Inverse(extractedLocal);
		BMath::BMatrix3x3 diffInRotation = extractedWorld * inverseLocal;

		for (int i = 0; i < meshComponent->mesh.meshPrimitives.size(); i++)
		{
			meshComponent->mesh.meshPrimitives[i].normals[0] = diffInRotation * meshComponent->mesh.meshPrimitives[i].normals[0];
		}
	}


	BMath::Vector3 RotationSystem::GetForwardVector(const BMath::BMatrix4x4& transformMatrix)
	{
		return  BMath::Vector3(-transformMatrix._13, -transformMatrix._23, -transformMatrix._33).Normalize();
	}

	BMath::Vector3 RotationSystem::GetUpVector(const BMath::BMatrix4x4& transformMatrix)
	{
		return  BMath::Vector3(transformMatrix._12, -transformMatrix._22, -transformMatrix._32).Normalize();
	}

	BMath::Vector3 RotationSystem::GetRightVector(const BMath::BMatrix4x4& transformMatrix)
	{
		return BMath::Vector3(transformMatrix._11, transformMatrix._21, transformMatrix._31).Normalize();
	}
}
