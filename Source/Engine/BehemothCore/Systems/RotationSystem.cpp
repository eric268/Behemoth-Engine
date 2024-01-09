#include "pch.h"
#include "RotationSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Misc/Log.h""
#include "Misc/BColors.h"
#include "Misc/TransformHelper.h"
#include "Geometry/Primitives.h"

namespace Behemoth
{
	void RotationSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<RotationComponent, TransformComponent>();

		if (doOnce)
		{
			doOnce = false;
			for (int i = 0; i < 12; i++)
			{
				entityHandles[i] = registry.CreateEntity("Debug Normals" + std::to_string(i));
				registry.AddComponent<DebugLineComponent>(entityHandles[i]);
			}
		}

		// DEBUG
		counter = 0;

		for (const auto& [entity, rotationComp, transformComp] : components)
		{
  			if (BMath::Quaternion::Equals(rotationComp->quat, BMath::Quaternion::Identity()) && !transformComp->parentIsDirty)
  			{
  				continue;
	  		}

			BMath::Matrix4x4 rotationMatrix = BMath::Quaternion::QuaternionToMatrix(rotationComp->quat);
			rotationComp->quat = BMath::Quaternion::Identity();

			// Ensure local transform is updated first
			ApplyRotation(transformComp->localTransform, rotationMatrix);
			UpdateWorldRotation(registry, entity, transformComp, rotationMatrix);
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

// 				for (int i = 0; i < 6; i++)
// 				{
// 					if (auto component = registry.GetComponent<DebugLineComponent>(entityHandles[counter++]))
// 					{
// 						BMath::Vector3 origin = GetPrimitivePosition(&meshComp->mesh.meshPrimitives[i]);
// 						BMath::Vector3 endPos = origin + meshComp->mesh.meshPrimitives[i].normals[0] * 2.0f;
// 
// 						auto c = BColors::colors[i];
// 
// 						component->lifetime = 100.0f;
// 						component->startPoint = origin;
// 						component->endPoint = endPos;
// 						component->color = c;
// 					}
// 				}
			}
		}
	}

	void RotationSystem::ApplyRotation(BMath::Matrix4x4& transform, const BMath::Matrix4x4& rotationMatrix)
	{
		BMath::Matrix4x4 rotatedTransformMatrix = rotationMatrix * transform;
		for (int col = 0; col < 3; col++)
		{
			for (int row = 0; row < 3; row++)
			{
				transform.data[col][row] = rotatedTransformMatrix.data[col][row];
			}
		}
	}

	void RotationSystem::UpdateWorldRotation(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, const BMath::Matrix4x4& rotationMatrix)
	{
		if (transformComp->parentIsDirty)
		{
			transformComp->worldTransform = TransformHelper::GetWorldTransform(registry, handle, transformComp->localTransform);
			transformComp->parentIsDirty = false;
		}
		else
		{
			ApplyRotation(transformComp->worldTransform, rotationMatrix);
		}

		transformComp->forwardVector = GetForwardVector(transformComp->worldTransform);
		transformComp->rightVector = GetRightVector(transformComp->worldTransform);
		transformComp->upVector = BMath::Vector3::Up();
	}


	void RotationSystem::RotateMeshNormals(MeshComponent* meshComponent, const BMath::Matrix4x4& rotationMatrix)
	{
		for (int i = 0; i < meshComponent->mesh.meshPrimitives.size(); i++)
		{
			// This method is faster than recalulating the normal, but this rotation matrix does not take into account parent rotation
			// Need to check if gettings the mesh normal from resource manager, then multiplying by world transform, then setting primitive normal is still faster
			// BMath::Vector3::RotateVector(meshComponent->mesh.meshPrimitives[i].normals[0], rotationMatrix, 0.0f);

			Primitives& primitive = meshComponent->mesh.meshPrimitives[i];
			primitive.normals[0] = BMath::Vector3(BMath::Vector4::Cross(primitive.verticies[1] - primitive.verticies[0], primitive.verticies[2] - primitive.verticies[0]));
			primitive.normals[0].Normalize();
		}
	}

	void RotationSystem::UpdateMeshNormalsFromParentRotation(TransformComponent* transformComp, MeshComponent* meshComponent)
	{
		BMath::Matrix3x3 extractedLocal = TransformHelper::ExtractRotationMatrix(transformComp->localTransform);
		BMath::Matrix3x3 extractedWorld = TransformHelper::ExtractRotationMatrix(transformComp->worldTransform);


		BMath::Matrix3x3 inverseLocal = BMath::Matrix3x3::Inverse(extractedLocal);
		BMath::Matrix3x3 diffInRotation = extractedWorld * inverseLocal;

		for (int i = 0; i < meshComponent->mesh.meshPrimitives.size(); i++)
		{
			meshComponent->mesh.meshPrimitives[i].normals[0] = diffInRotation * meshComponent->mesh.meshPrimitives[i].normals[0];
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

	BMath::Vector3 RotationSystem::GetPrimitivePosition(Primitives* primitive)
	{
		int numVertices = static_cast<PrimitiveType>(primitive->primitiveType);
		if (numVertices <= 0)
			return BMath::Vector3{};

		BMath::Vector3 averagePos{};

		for (int i = 0; i < numVertices; i++)
		{
			averagePos += BMath::Vector3(primitive->verticies[i]) / static_cast<float>(numVertices);
		}
		return averagePos;
	}
}
