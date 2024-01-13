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
				UpdateFrustrum(cameraComp, transformComp);
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

		const float fovScale = 1.0f / (std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f));
		const float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;
		const float farPlane = cameraComponent->farClippingPlane;
		const float nearPlane = cameraComponent->nearClippingPlane;

		cameraComponent->projMatrix = BMath::Matrix4x4::Zero();
		cameraComponent->projMatrix._11 = fovScale / aspectRatio;
		cameraComponent->projMatrix._22 = fovScale;
		cameraComponent->projMatrix._33 = -(farPlane + nearPlane) / (farPlane - nearPlane);
		cameraComponent->projMatrix._43 = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
		cameraComponent->projMatrix._34 = -1.0f;

		BMath::Vector3 target = transformComponent->worldPosition + transformComponent->forwardVector;

		cameraComponent->viewMatrix = CameraHelper::LookAt(transformComponent->worldPosition, target, BMath::Vector3(0,1,0));
		cameraComponent->inverseTransposeViewMatrix = BMath::Matrix4x4::Transpose(BMath::Matrix4x4::Inverse(cameraComponent->viewMatrix));
	}

	void CameraSystem::UpdateFrustrum(CameraComponent* cameraComponent, TransformComponent* transformComp)
	{
		using namespace BMath;

		float nearDist = cameraComponent->nearClippingPlane;
 		float farDist = cameraComponent->farClippingPlane;
		Vector3 p = transformComp->worldPosition;
	 	Vector3 d = transformComp->forwardVector;
		Vector3 up = BMath::Vector3(0,1,0);
 		Vector3 right = transformComp->rightVector;

		const float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;

		float nearD = cameraComponent->nearClippingPlane;
		float farD = cameraComponent->farClippingPlane;

		float tang = std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f);
		float nearHeight = nearD * tang;
		float nearWidth = nearHeight * aspectRatio;
		float fh = farD * tang;
		float fw = fh * aspectRatio;

		Vector3 X, Y, Z;
		Vector3 l = p + d;
		Z = (p - l).Normalize();
		X = Vector3::Cross(Vector3::Up(), Z);
		Y = Vector3::Cross(Z, X);

		Vector3 nc = p - Z * nearD;
		Vector3 fc = p + Z * farD;

		Vector3 aux, normal;

		// Left
		aux = ((nc - X * nearWidth) - p).Normalize();
		normal = Vector3::Cross(aux, Y);
		cameraComponent->worldSpaceFrustum[0].normal = normal;
		cameraComponent->worldSpaceFrustum[0].d = -Vector3::Dot(nc - X * nearWidth, normal);

		// Right
		aux = ((nc + X * nearWidth) - p).Normalize();
		normal = Vector3::Cross(Y, aux);
		cameraComponent->worldSpaceFrustum[1].normal = normal;
		cameraComponent->worldSpaceFrustum[1].d = -Vector3::Dot(nc + X * nearWidth, normal);

		// Bottom
		aux = ((nc - Y * nearHeight) - p).Normalize();
		normal = Vector3::Cross(X, aux);
		cameraComponent->worldSpaceFrustum[2].normal = normal;
		cameraComponent->worldSpaceFrustum[2].d = -Vector3::Dot(nc - Y * nearHeight, normal);

		// Top
		aux = ((nc + Y * nearHeight) - p).Normalize();
		normal = Vector3::Cross(aux, X);
		cameraComponent->worldSpaceFrustum[3].normal = normal;
		cameraComponent->worldSpaceFrustum[3].d = -Vector3::Dot(nc + Y * nearHeight, normal);

		// Near
		cameraComponent->worldSpaceFrustum[4].normal = -Z;
		cameraComponent->worldSpaceFrustum[4].d = -Vector3::Dot(nc, normal);

		// Far
		cameraComponent->worldSpaceFrustum[5].normal = Z;
		cameraComponent->worldSpaceFrustum[5].d = -Vector3::Dot(fc, normal);

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



// 		auto m =  cameraComponent->projMatrix * cameraComponent->viewMatrix;
// 
// 		cameraComponent->worldSpaceFrustum[0].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(0)).Normalize();
// 		cameraComponent->worldSpaceFrustum[0].d = (m.data[3][3] + m.data[0][3]);
// 
// 		cameraComponent->worldSpaceFrustum[1].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(0)).Normalize();
// 		cameraComponent->worldSpaceFrustum[1].d = (m.data[3][3] - m.data[0][3]);
// 
// 		cameraComponent->worldSpaceFrustum[2].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(1)).Normalize();
// 		cameraComponent->worldSpaceFrustum[2].d = (m.data[3][3] + m.data[1][3]);
// 
// 		cameraComponent->worldSpaceFrustum[3].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(1)).Normalize();
// 		cameraComponent->worldSpaceFrustum[3].d = (m.data[3][3] - m.data[1][3]);
// 
// 		cameraComponent->worldSpaceFrustum[4].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(2)).Normalize();
// 		cameraComponent->worldSpaceFrustum[4].d = cameraComponent->nearClippingPlane;
// 
// 		cameraComponent->worldSpaceFrustum[5].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(2)).Normalize();
// 		cameraComponent->worldSpaceFrustum[5].d = cameraComponent->farClippingPlane;