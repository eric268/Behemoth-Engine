

#include "System.h"
#include "Components.h"
#include "Application/ResourceManager.h"
#include "Render/Mesh.h"
#include "Misc/Log.h"
#include "Math/MathCore.h"
#include "Misc/CameraHelper.h"

#include <Windows.h>
#include "main.h"
#include <algorithm>
#include <execution>

using namespace BehemothEngine;
namespace ECS
{
	void RenderSystem::Run(Registry& registry)
	{
		auto components = registry.Get<MeshComponent, TransformComponent>();
		auto cameraComponents = registry.Get<CameraComponent, TransformComponent>();

		CameraComponent* mainCamera = nullptr;
		TransformComponent* mainCameraTransform = nullptr;

		for (const auto& [cameraComp, transformComp] : cameraComponents)
		{
			if (cameraComp->isMain)
			{
				mainCamera = cameraComp;
				mainCameraTransform = transformComp;
				break;
			}
		}

		if (!mainCamera || !mainCameraTransform)
		{
			LOG_ERROR("Main camera not found");
			return;
		}

		for (const auto& [meshComp, transformComp] : components)
		{
			if (!meshComp || !transformComp)
			{
				LOG_ERROR("Null component found");
				continue;
			}

			// ** Order of multiplication matters here **
			auto combinedMatrix = transformComp->transformMatrix;
			Math::Matrix4x4 viewProjMatrix = mainCamera->viewMatrix * mainCamera->perspectiveMatrix;


			std::size_t size = meshComp->mesh.meshPrimitives.size();
			for (int i = 0; i < size; i++)
			{
				Math::Vector4 vertex[3];

				// originally was a for loop but after profiling unrolling the loop lead to a 2.5x increase in CPU time
				vertex[0] = Math::Vector4(meshComp->mesh.meshPrimitives[i].verticies[0], 1.0f);
 				vertex[0] = vertex[0] * transformComp->transformMatrix;
 				vertex[0] = vertex[0] * viewProjMatrix;
 
 				vertex[1] = Math::Vector4(meshComp->mesh.meshPrimitives[i].verticies[1], 1.0f);
 				vertex[1] = vertex[1] * transformComp->transformMatrix;
 				vertex[1] = vertex[1] * viewProjMatrix;
 
 				vertex[2] = Math::Vector4(meshComp->mesh.meshPrimitives[i].verticies[2], 1.0f);
 				vertex[2] = vertex[2] * transformComp->transformMatrix;
 				vertex[2] = vertex[2] * viewProjMatrix;

				vertex[0] = vertex[0] / vertex[0].w;
				vertex[1] = vertex[1] / vertex[1].w;
				vertex[2] = vertex[2] / vertex[2].w;

				if (CullBackFace(mainCameraTransform->position, vertex))
				{
					continue;
				}

				meshComp->mesh.meshPrimitives[i].SetSpriteVerticies(vertex, meshComp->mesh.meshPrimitives[i].uv);
				// meshComp->mesh.meshPrimitives[i].DrawWireMesh();
				meshComp->mesh.meshPrimitives[i].Draw();
			}
		}
	}

	bool RenderSystem::CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[3])
	{

		const Math::Vector3 normal = Math::Vector3(Math::Vector4::Cross(primitiveVerts[1] - primitiveVerts[0], primitiveVerts[2] - primitiveVerts[0]));
		const Math::Vector3 cam = cameraLocation - Math::Vector3(primitiveVerts[0]);
		return (Math::Vector3::Dot(normal, cam)) <= 0;
	}

	void ModelInitalizeSystem::Run(Registry& registry)
	{
		// Will need to get the sparse set container no matter what since the .Get<>() returns a tuple and iterating backwards is difficult
		// This way we can do a very quick and easy check to see if there are any meshes that need to be initialized
		// most likely this check will fail and exit early so want to do this before calling .Get on the registry

		auto eraseInitComponents = registry.GetComponent<MeshInitalizeComponent>();
		if (!eraseInitComponents->size())
			return;

		auto components = registry.Get<MeshComponent>();
		for (const auto& [ meshComp] : components)
		{
			const MeshData* data = ResourceManager::GetInstance().GetMesh(meshComp->modelFileName);

			if (data)
			{
				meshComp->mesh.GeneratePrimitives(data->meshData);
			}
			else
			{
				LOG_ERROR("Error loading mesh");
			}
		}

		// Iterate over the component backwards because we always swap a component to the back of the container before removing it
		// iterating over backwards allows us to update the container size and not cause errors in the range of the loop
		for (int i = eraseInitComponents->dense.size() - 1; i >= 0; i--)
		{
			eraseInitComponents->RemoveComponent(eraseInitComponents->dense[i]);
		}
	}

	void CameraSystem::Run(Registry& registry)
	{
		auto components = registry.Get<CameraComponent, TransformComponent>();

		for (auto& [cameraComp, transformComp] : components)
		{
			UpdatePerspectiveMatrix(*cameraComp, transformComp->position);
		}
	}

	void CameraSystem::UpdatePerspectiveMatrix(CameraComponent& component, const Math::Vector3& position)
	{
		RECT rect;
		GetClientRect(MAIN_WINDOW_HANDLE, &rect);
		component.windowWidth = rect.right - rect.left;
		component.windowHeight = rect.bottom - rect.top;

		const float fovScale = 1.0f / (std::tan(DEGREE_TO_RAD(component.FOV) * 0.5f));
		const float aspectRatio = component.windowHeight / component.windowWidth;
		const float farPlane = component.farClippingPlane;
		const float nearPlane = component.nearClippingPlane;

		component.perspectiveMatrix = Math::Matrix4x4::Zero();

		component.perspectiveMatrix[0][0] = fovScale * aspectRatio;
		component.perspectiveMatrix[1][1] = fovScale;
		component.perspectiveMatrix[2][2] = farPlane/ (farPlane - nearPlane);
		component.perspectiveMatrix[2][3] = (-farPlane * nearPlane) / (farPlane - nearPlane);
		component.perspectiveMatrix[3][2] = 1.0f;
		component.perspectiveMatrix[3][3] = 0.0f;

		component.viewMatrix = CameraHelper::LookAt(position, Math::Vector3(0, 0, 0), Math::Vector3(0, 1, 0));
	}

	void RotationSystem::Run(Registry& registry)
	{
		auto components = registry.Get<RotationComponent, TransformComponent, MeshComponent>();

		for (const auto& [rotationComp, transformComp, meshComp] : components)
		{
			if (!rotationComp || !transformComp || !meshComp)
			{
				LOG_ERROR("Null component found");
				continue;
			}

			Math::Matrix4x4 rotationMatrix = Math::Matrix4x4::GetRotationMatrix(rotationComp->axis, rotationComp->speed * 0.01667f);
			Math::Matrix4x4 newMatrix = transformComp->transformMatrix * rotationMatrix;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					transformComp->transformMatrix[i][j] = newMatrix[i][j];
					
				}
			}
			

			for (int i = 0; i < meshComp->mesh.meshPrimitives.size(); i++)
			{
				for (int j = 0; j < 3; j++)
				{
					Math::Vector3::RotateVector(meshComp->mesh.meshPrimitives[i].normals[j],  rotationMatrix);
				}
				
			}
		}
	}

	void MovementSystem::Run(Registry& registry)
	{
		auto movementComponents = registry.GetComponent<MovementComponent>();

		if (!movementComponents->size())
			return;

		auto components = registry.Get<MovementComponent, TransformComponent>();
		for (const auto& [movementComp, transformComp] : components)
		{
			transformComp->position += movementComp->location;
			transformComp->transformMatrix[3][0] += movementComp->location.x * WORLD_SCALE;
			transformComp->transformMatrix[3][1] += movementComp->location.y * WORLD_SCALE;
			transformComp->transformMatrix[3][2] += movementComp->location.z * WORLD_SCALE;
		}

		for (int i = movementComponents->dense.size() - 1; i >= 0; i--)
		{
			movementComponents->RemoveComponent(movementComponents->dense[i]);
		}
	}
}