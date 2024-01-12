#include "pch.h"
#include "CameraSystem.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"
#include "NextAPI/App/main.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	void CameraSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<CameraComponent, TransformComponent>();

		for (auto& [entity, cameraComp, transformComp] : components)
		{
			if (cameraComp->isDirty)
			{
				UpdatePerspectiveMatrix(cameraComp, transformComp);
				UpdateFrustrum(cameraComp);
				cameraComp->isDirty = false;
			}
		}
	}

	void CameraSystem::UpdatePerspectiveMatrix(CameraComponent* cameraComponent, const TransformComponent* transformComponent)
	{
		RECT rect;
		GetClientRect(MAIN_WINDOW_HANDLE, &rect);
		cameraComponent->windowWidth = rect.right - rect.left;
		cameraComponent->windowHeight = rect.bottom - rect.top;

		// std::cout << "Width: " << cameraComponent->windowWidth << ' ' << " Height: " << cameraComponent->windowHeight << std::endl;

		const float fovScale = 1.0f / (std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f));
		const float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;
		const float farPlane = cameraComponent->farClippingPlane;
		const float nearPlane = cameraComponent->nearClippingPlane;

// 		cameraComponent->projMatrix = BMath::Matrix4x4::Zero();
// 		cameraComponent->projMatrix._11 = fovScale / aspectRatio;
// 		cameraComponent->projMatrix._22 = fovScale;
// 		cameraComponent->projMatrix._33 = (farPlane) / (farPlane - nearPlane); // maybe switch these two
// 		cameraComponent->projMatrix._43 = (-farPlane * nearPlane) / (farPlane - nearPlane); // maybe switch these two
// 		cameraComponent->projMatrix._34 = 1.0f;

		cameraComponent->projMatrix = BMath::Matrix4x4::Zero();
		cameraComponent->projMatrix._11 = fovScale / aspectRatio;
		cameraComponent->projMatrix._22 = fovScale;
		cameraComponent->projMatrix._33 = (-farPlane + nearPlane) / (farPlane - nearPlane);
		cameraComponent->projMatrix._43 = -2.0f * (farPlane * nearPlane) / (farPlane - nearPlane);
		cameraComponent->projMatrix._34 = -1.0f;
		cameraComponent->projMatrix._34 = -1.0f;
		cameraComponent->projMatrix._44 = 1.0f;

		BMath::Vector3 target = transformComponent->worldPosition + transformComponent->forwardVector;


		cameraComponent->viewMatrix = CameraHelper::LookAt(transformComponent->worldPosition, target, BMath::Vector3(0,1,0));
		cameraComponent->inverseTransposeViewMatrix =BMath::Matrix4x4::Inverse(cameraComponent->viewMatrix);
	}

	void CameraSystem::UpdateFrustrum(CameraComponent* cameraComponent)
	{
		auto m =  cameraComponent->projMatrix * cameraComponent->viewMatrix;

		// cameraComponent->worldSpaceFrustum[0].normal = BMath::Vector3(m.data[3][0] + m.data[0][0], m.data[3][1] + m.data[0][1], m.data[3][2] + m.data[0][2]).Normalize();
		cameraComponent->worldSpaceFrustum[0].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(0)).Normalize();
		cameraComponent->worldSpaceFrustum[0].distance = (m.data[3][3] + m.data[0][3]);

		// cameraComponent->worldSpaceFrustum[1].normal = BMath::Vector3(m.data[3][0] - m.data[0][0], m.data[3][1] - m.data[0][1], m.data[3][2] - m.data[0][2]).Normalize();
		cameraComponent->worldSpaceFrustum[1].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(0)).Normalize();
		cameraComponent->worldSpaceFrustum[1].distance = (m.data[3][3] - m.data[0][3]);

		// cameraComponent->worldSpaceFrustum[2].normal = BMath::Vector3(m.data[3][0] + m.data[1][0], m.data[3][1] + m.data[1][1], m.data[3][2] + m.data[1][2]).Normalize();
		cameraComponent->worldSpaceFrustum[2].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(1)).Normalize();
		cameraComponent->worldSpaceFrustum[2].distance = (m.data[3][3] + m.data[1][3]);

		// cameraComponent->worldSpaceFrustum[3].normal = BMath::Vector3(m.data[3][0] - m.data[1][0], m.data[3][1] - m.data[1][1], m.data[3][2] - m.data[1][2]).Normalize();
		cameraComponent->worldSpaceFrustum[3].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(1)).Normalize();
		cameraComponent->worldSpaceFrustum[3].distance = (m.data[3][3] - m.data[1][3]);

		// cameraComponent->worldSpaceFrustum[4].normal = BMath::Vector3(m.data[3][0] + m.data[2][0], m.data[3][1] + m.data[2][1], m.data[3][2] + m.data[2][2]).Normalize();
		cameraComponent->worldSpaceFrustum[4].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(2)).Normalize();
		cameraComponent->worldSpaceFrustum[4].distance = (m.data[3][3] + m.data[2][3]);

		// cameraComponent->worldSpaceFrustum[5].normal = BMath::Vector3(m.data[3][0] - m.data[2][0], m.data[3][1] - m.data[2][1], m.data[3][2] - m.data[2][2]).Normalize();
		cameraComponent->worldSpaceFrustum[5].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(2)).Normalize();
		cameraComponent->worldSpaceFrustum[5].distance = (m.data[3][3] - m.data[2][3]);

// 		float thetaY = DEGREE_TO_RAD(cameraComponent->FOV);
// 		float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;
// 		float thetaX = 2.0f * std::atan(std::tan(thetaY * 0.5f) / aspectRatio);
// 
// 		thetaY *= 0.5f;
// 		thetaX *= 0.5f;
// 
//   		// Rotate around y axis
//  		BMath::Vector3 leftNormal = BMath::Vector3(std::sin(thetaX), 0.0f, -std::cos(thetaX)).Normalize();
//  		cameraComponent->worldSpaceFrustum[0] = Plane::TransformPlane(Plane(leftNormal, cameraComponent->nearClippingPlane), cameraComponent->inverseTransposeViewMatrix);
// 		//cameraComponent->worldSpaceFrustum[0].distance = GetDistance(cameraComponent->inverseTransposeViewMatrix, 0);
// 
// 
//  		// Rotate around y axis
//  		BMath::Vector3 rightNormal = BMath::Vector3(-std::sin(thetaX), 0.0f, -std::cos(thetaX)).Normalize();
//  		cameraComponent->worldSpaceFrustum[1] = Plane::TransformPlane(Plane(rightNormal, cameraComponent->nearClippingPlane), cameraComponent->inverseTransposeViewMatrix);
// 		//cameraComponent->worldSpaceFrustum[1].distance = GetDistance(cameraComponent->inverseTransposeViewMatrix, 1);
// 
//  		// Rotate around x axis
//  		BMath::Vector3 bottomNormal = BMath::Vector3(0.0f, std::cos(thetaY), -std::sin(thetaY)).Normalize();
//  		cameraComponent->worldSpaceFrustum[2] = Plane::TransformPlane(Plane(bottomNormal, cameraComponent->nearClippingPlane), cameraComponent->inverseTransposeViewMatrix);
// 		//cameraComponent->worldSpaceFrustum[2].distance = -GetDistance(cameraComponent->inverseTransposeViewMatrix, 2);
// 
//  		// Rotate around x axis
//  		BMath::Vector3 topNormal = BMath::Vector3(0.0f, -std::cos(thetaY), -std::sin(thetaY)).Normalize();
//  		cameraComponent->worldSpaceFrustum[3] = Plane::TransformPlane(Plane(topNormal, cameraComponent->nearClippingPlane), cameraComponent->inverseTransposeViewMatrix);
// 		//cameraComponent->worldSpaceFrustum[3].distance = -GetDistance(cameraComponent->inverseTransposeViewMatrix, 3);
// 
//  		BMath::Vector3 nearNormal = BMath::Vector3(0.0f, 0.0f, -1.0f);
//  		//cameraComponent->worldSpaceFrustum[4] = Plane::TransformPlane(Plane(nearNormal, cameraComponent->nearClippingPlane), cameraComponent->inverseTransposeViewMatrix);
// 
//  		BMath::Vector3 farNormal = BMath::Vector3(0.0f, 0.0f, 1.0f);
//  		//cameraComponent->worldSpaceFrustum[5] = Plane::TransformPlane(Plane(farNormal, cameraComponent->farClippingPlane), cameraComponent->inverseTransposeViewMatrix);
	}
	float CameraSystem::GetDistance(const BMath::Matrix4x4& m, int index)
	{
		if (index > 3)
		{
			index = 0;
		}

		static BMath::Vector3 normals[4] = 
		{
			BMath::Vector3(1,0,0), 
			BMath::Vector3(-1,0,0), 
			BMath::Vector3(0,1,0), 
			BMath::Vector3(0,-1,0)
		};

		BMath::Vector4 transformedNormal = m * BMath::Vector4(normals[index], 1.0f);
		BMath::Vector3 result = BMath::Vector3(transformedNormal) / transformedNormal.w;
		return BMath::Vector3::Magnitude(result);
	}
}