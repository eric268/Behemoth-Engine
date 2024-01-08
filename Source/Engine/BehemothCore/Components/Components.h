#pragma once

#include "ECS/Component.h"
#include "Input/InputCodes.h"
#include "Geometry/Mesh.h"
#include "Geometry/Plane.h"
#include "Math/Quaternion.h"

#include <functional>
#include <unordered_map>

namespace Behemoth
{
	struct CameraComponent : public ECS::Component
	{
		CameraComponent(bool main = false) :
			viewMatrix(BMath::Matrix4x4::Identity()),
			perspectiveMatrix(BMath::Matrix4x4::Identity()),
			FOV(75.0f),
			nearClippingPlane(0.1f),
			farClippingPlane(1000.0f),
			windowWidth(0.0f),
			windowHeight(0.0f),
			isMain(main),
			isDirty(true)
			{}

		~CameraComponent() = default;
		Plane worldSpaceFrustum[6];
		BMath::Matrix4x4 viewMatrix;
		BMath::Matrix4x4 perspectiveMatrix;
		BMath::Matrix4x4 inverseTransposeViewMatrix;
		float FOV;
		float nearClippingPlane;
		float farClippingPlane;
		float windowWidth;
		float windowHeight;
		bool isMain;
		bool isDirty;
	};

	struct MeshInitalizeComponent : public ECS::Component
	{
		MeshInitalizeComponent() = default;
	};

	struct TransformComponent : public ECS::Component
	{
		TransformComponent() : transformMatrix(BMath::Matrix4x4::Identity()), isDirty(true), forwardVector(BMath::Vector3::Forward()), rightVector(BMath::Vector3::Right()), upVector(BMath::Vector3::Up()) {}

		BMath::Matrix4x4 transformMatrix;
		BMath::Vector3 forwardVector;
		BMath::Vector3 rightVector;
		BMath::Vector3 upVector;
		BMath::Vector3 position;
		BMath::Vector3 scale;
		bool isDirty;
	};

	struct StaticComponent : public ECS::Component
	{
	public:
		StaticComponent() = default;
	};

	struct MoveComponent : public ECS::Component
	{
		MoveComponent() : location(BMath::Vector3{}) {}
		MoveComponent(BMath::Vector3 vec) : location(vec) {}

		BMath::Vector3 location;
	};

	struct VelocityComponent : public ECS::Component
	{
		VelocityComponent() : velocity(BMath::Vector3{}) {}
		VelocityComponent(BMath::Vector3 vel) : velocity(vel) {}

		BMath::Vector3 velocity;
	};

	struct RotationComponent : public ECS::Component
	{
		RotationComponent() : quat() {}
		RotationComponent(BMath::Quaternion q) : quat(q) {}
		BMath::Quaternion quat;
		BMath::Vector3 eulerAngles;
	};

	struct ScalingComponent : public ECS::Component
	{
		ScalingComponent() : scalingVector(BMath::Vector3(1.0f, 1.0f, 1.0f)) {}
		ScalingComponent(BMath::Vector3 vec) : scalingVector(vec) {}

		BMath::Vector3 scalingVector;
	};

	struct ChildComponent : public ECS::Component
	{
		std::uint32_t parentID;
	};

	struct ParentComponent : public ECS::Component
	{
		std::vector<std::uint32_t> childrenIDs;
	};
}