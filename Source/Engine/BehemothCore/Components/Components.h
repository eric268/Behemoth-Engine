#pragma once

#include "ECS/Component.h"
#include "Input/InputCodes.h"
#include "Geometry/Mesh.h"
#include "Geometry/Plane.h"
#include "Math/Quaternion.h"
#include "ECS/Entity.h"
#include "Math/Matrix.h"

#include <functional>
#include <unordered_map>
#include <vector>

namespace Behemoth
{
	struct CameraComponent : public ECS::Component
	{
		CameraComponent(bool main = false) :
			viewMatrix(BMath::Matrix4x4<float>::Identity()),
			projMatrix(BMath::Matrix4x4<float>::Identity()),
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
		BMath::Matrix4x4<float> viewMatrix;
		BMath::Matrix4x4<float> projMatrix;
		BMath::Matrix4x4<float> inverseTransposeViewMatrix;
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
		MeshInitalizeComponent(bool initBoundingVolume = true, bool initBroadCollider = true) : 
			initBoundingVolume(initBoundingVolume), 
			initBroadCollider(initBroadCollider) 
		{}

		bool initBoundingVolume;
		bool initBroadCollider;
	};

	struct TransformComponent : public ECS::Component
	{
		TransformComponent() :
			worldTransform(BMath::Matrix4x4<float>::Identity()),
			localTransform(BMath::Matrix4x4<float>::Identity()),
			forwardVector(BMath::Vector3::Forward()),
			rightVector(BMath::Vector3::Right()),
			upVector(BMath::Vector3::Up()),
			worldPosition(BMath::Vector3(0.0f)),
			localPosition(BMath::Vector3(0.0f)),
			worldScale(BMath::Vector3(1.0f)),
			localScale(BMath::Vector3(1.0f)),
			isDirty(true),
			parentIsDirty(false)
		{}

		BMath::Matrix4x4<float> worldTransform;
		BMath::Matrix4x4<float> localTransform;
		BMath::Vector3 forwardVector;
		BMath::Vector3 rightVector;
		BMath::Vector3 upVector;
		BMath::Vector3 worldPosition;
		BMath::Vector3 localPosition;
		BMath::Vector3 worldScale;
		BMath::Vector3 localScale;
		bool isDirty;
		bool parentIsDirty;
	};

	struct StaticComponent : public ECS::Component
	{
	public:
		StaticComponent() = default;
	};

	struct MoveComponent : public ECS::Component
	{
		MoveComponent() : location(BMath::Vector3(0.0f)) {}
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
		ChildComponent() : parentHandle(NULL_ENTITY) {}
		explicit ChildComponent(ECS::EntityHandle handle) : parentHandle(handle) {}
		ECS::EntityHandle parentHandle;
	};

	struct ParentComponent : public ECS::Component
	{
		ParentComponent() : childHandles(std::vector<ECS::EntityHandle>()) {}

		std::vector<ECS::EntityHandle> childHandles;
	};

	struct TimerComponent : public ECS::Component
	{
		TimerComponent() : duration(-1.0f), counter(0.0f), funcPtr(nullptr) {}
		TimerComponent(std::function<void()> func, float time) : duration(time), counter(0.0f), funcPtr(func) {}

		float counter;
		float duration;
		std::function<void()> funcPtr;
	};
}