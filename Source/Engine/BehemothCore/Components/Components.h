#include "ECS/Component.h"
#include "Input/InputCodes.h"
#include "Renderer/Mesh.h"

#include <functional>
#include <unordered_map>

namespace Behemoth
{
	struct CameraComponent : public ECS::Component
	{
		CameraComponent(bool main = false) :
			viewMatrix(Math::Matrix4x4::Identity()),
			perspectiveMatrix(Math::Matrix4x4::Identity()),
			FOV(75.0f),
			nearClippingPlane(0.1f),
			farClippingPlane(1000.0f),
			windowWidth(0.0f),
			windowHeight(0.0f),
			isMain(main),
			isDirty(true)
			{}

		~CameraComponent() = default;
		Math::Plane worldSpaceFrustum[6];
		Math::Matrix4x4 viewMatrix;
		Math::Matrix4x4 perspectiveMatrix;
		Math::Matrix4x4 inverseTransposeViewMatrix;
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
		TransformComponent() : transformMatrix(Math::Matrix4x4::Identity()), isDirty(true), forwardVector(Math::Vector3::Forward()), rightVector(Math::Vector3::Right()), upVector(Math::Vector3::Up()) {}

		Math::Matrix4x4 transformMatrix;
		Math::Vector3 forwardVector;
		Math::Vector3 rightVector;
		Math::Vector3 upVector;
		Math::Vector3 position;
		Math::Vector3 scale;
		bool isDirty;
	};

	struct MoveComponent : public ECS::Component
	{
		MoveComponent() : location(Math::Vector3{}) {}
		MoveComponent(Math::Vector3 vec) : location(vec) {}

		Math::Vector3 location;
	};

	struct VelocityComponent : public ECS::Component
	{
		VelocityComponent() : velocity(Math::Vector3{}) {}
		VelocityComponent(Math::Vector3 vel) : velocity(vel) {}

		Math::Vector3 velocity;
	};

	struct RotationComponent : public ECS::Component
	{
		enum Axis
		{
			None,
			X_AXIS,
			Y_AXIS,
			Z_AXIS
		};

		RotationComponent() : axis(None), speed(0.0f) {}
		RotationComponent(Axis a, float s) : axis(a), speed(s) {}
		RotationComponent(int a, float s) : axis(static_cast<Axis>(a)), speed(s) {}

		Axis axis;
		float speed;
	};

	struct ScalingComponent : public ECS::Component
	{
		ScalingComponent() : scalingVector(Math::Vector3(1.0f, 1.0f, 1.0f)) {}
		ScalingComponent(Math::Vector3 vec) : scalingVector(vec) {}

		Math::Vector3 scalingVector;
	};
}