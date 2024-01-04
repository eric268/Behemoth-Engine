#include "ECS/Component.h"
#include "Input/InputCodes.h"
#include "Renderer/Mesh.h"

#include <functional>
#include <unordered_map>

namespace Behemoth
{
	struct MeshComponent : public ECS::Component
	{
		MeshComponent(const std::string& modelName, const std::string& textureName, const Math::Vector2 uvScale = { 1.0f,1.0f }, bool visible = true) :
		modelFileName(modelName), 
			textureFileName(textureName), 
			mesh(modelName, textureName, uvScale),
			isVisible(visible)
			{}

		bool isVisible;

		Mesh mesh;
		std::string modelFileName;
		std::string textureFileName;
	};

	struct WireframeComponent : public ECS::Component
	{
		WireframeComponent(const std::string& modelName, Math::Vector3 scale = Math::Vector3::One(), bool visible = false, Math::Vector3 color = Math::Vector3(1.0f, 1.0f, 1.0f)) : modelFileName(modelName), scale(scale), isVisible(visible), mesh(modelName), wireframeColor(color) {}
		std::string modelFileName;
		Math::Vector3 scale;
		bool isVisible;
		Math::Vector3 wireframeColor;
		Mesh mesh;
	};

	struct BoundingVolumeComponent : public ECS::Component
	{
		BoundingVolumeComponent() : mesh("sphere.obj"), volumeRadius(1.0f), isVisible(false) {}
		BoundingVolumeComponent(float radius, bool visible) : mesh("sphere.obj"), volumeRadius(radius), isVisible(visible) {}
		float volumeRadius;
		bool isVisible;
		Mesh mesh;
	};

	struct MeshInitalizeComponent : public ECS::Component
	{
		MeshInitalizeComponent() = default;
	};

	struct HealthComponent : public ECS::Component
	{
		HealthComponent(int health) : currentHealth(health) {}

		int currentHealth;
	};

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

	struct TransformComponent : public ECS::Component
	{
		TransformComponent() : transformMatrix(Math::Matrix4x4::Identity()), isDirty(true), forwardVector(Math::Vector3::Forward()), rightVector(Math::Vector3::Right()) {}

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

	struct DirectionalLightComponent : public ECS::Component
	{
		DirectionalLightComponent() : 
			direction(Math::Vector3(-0.57f, -0.57f, 0.57f)),
			color(Math::Vector3(0.25f, 0.25f, 0.25f)),
			intensity(4.0f) {}

		DirectionalLightComponent(Math::Vector3 dir, Math::Vector3 color, float intensity) :
			direction(dir),
			color(color),
			intensity(intensity) {}

		Math::Vector3 direction;
		Math::Vector3 color;
		float intensity;
	};

	struct PointLightComponent : public ECS::Component
	{
		PointLightComponent() :
			color(Math::Vector3(0.25f, 0.25, 0.25f)),
			intensity(1.0f),
			constant(1.0f),
			linear(0.14f),
			quadratic(0.07f) 
		{}

		PointLightComponent(Math::Vector3 color, float intensity, float constant = 1.0f,  float linear = 0.14f, float quadratic = 0.07f) :
			color(color),
			intensity(intensity),
			constant(constant),
			linear(linear),
			quadratic(quadratic)
		{}

		Math::Vector3 color;
		float intensity;
		float constant;
		float linear;
		float quadratic;
	};
}