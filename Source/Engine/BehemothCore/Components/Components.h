#include "ECS/Component.h"

namespace Behemoth
{
	class MeshComponent : public ECS::Component
	{
	public:
		MeshComponent(const std::string& modelName, const std::string& textureName, const Math::Vector2 uvScale = { 1.0f,1.0f }, bool visible = true, bool drawWireMesh = false) :
		modelFileName(modelName), 
			textureFileName(textureName), 
			mesh(modelName, textureName, uvScale),
			isVisible(visible),
			drawWireMesh(drawWireMesh)
			{}

		~MeshComponent() override {};

		bool isVisible;
		bool drawWireMesh;

		Behemoth::Mesh mesh;
		std::string modelFileName;
		std::string textureFileName;
	};


	class MeshInitalizeComponent : public ECS::Component
	{
	public:
		MeshInitalizeComponent() = default;
	};

	class HealthComponent : public ECS::Component
	{
	public:
		HealthComponent(int health) : currentHealth(health) {}
		~HealthComponent() override {};

		int currentHealth;
	};

	class PhysicsComponent : public ECS::Component
	{
	public:
		PhysicsComponent() {}
		~PhysicsComponent() override {};
	};

	class CameraComponent : public ECS::Component
	{
	public:
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

	class TransformComponent : public ECS::Component
	{
	public:
		TransformComponent() : transformMatrix(Math::Matrix4x4::Identity()), dirty(true), forwardVector(Math::Vector3::Forward()), rightVector(Math::Vector3::Right()) {}
		Math::Matrix4x4 transformMatrix;
		Math::Vector3 forwardVector;
		Math::Vector3 rightVector;
		Math::Vector3 upVector;
		Math::Vector3 position;
		Math::Vector3 scale;
		bool dirty;
	};

	class MoveComponent : public ECS::Component
	{
	public:
		MoveComponent() : location(Math::Vector3{}) {}
		MoveComponent(Math::Vector3 vec) : location(vec) {}
		~MoveComponent() override {};

		Math::Vector3 location;
	};

	class VelocityComponent : public ECS::Component
	{
	public:
		VelocityComponent() : velocity(Math::Vector3{}) {}
		VelocityComponent(Math::Vector3 vel) : velocity(vel) {}

		Math::Vector3 velocity;
	};

	class RotationComponent : public ECS::Component
	{
	public:

		enum Axis
		{
			X_AXIS,
			Y_AXIS,
			Z_AXIS
		};

		RotationComponent() : axis(X_AXIS), speed(1.0f) {}
		RotationComponent(Axis a, float s) : axis(a), speed(s) {}
		RotationComponent(int a, float s) : axis(static_cast<Axis>(a)), speed(s) {}

		Axis axis;
		float speed;
	};

	class ScalingComponent : public ECS::Component
	{
	public:
		ScalingComponent() : scalingVector(Math::Vector3(1.0f, 1.0f, 1.0f)) {}
		ScalingComponent(Math::Vector3 vec) : scalingVector(vec) {}

		Math::Vector3 scalingVector;
	};


	class BoundingVolumeComponent : public ECS::Component 
	{
	public:
		BoundingVolumeComponent() : mesh("sphere.obj"), volumeRadius(1.0f), drawBoundingVolume(false) {}
		BoundingVolumeComponent(float radius, bool draw) : mesh("sphere.obj"), volumeRadius(radius), drawBoundingVolume(draw) {}
		float volumeRadius;
		bool drawBoundingVolume;
		Behemoth::Mesh mesh;
	};

	class DirectionalLightComponent : public ECS::Component 
	{
	public:
		DirectionalLightComponent() : 
			direction(Math::Vector3(0.0f, 0.0f, -1.0f)), 
			color(Math::Vector3(0.25f, 0.25, 0.25f)),
			intensity(1.0f) {}

		DirectionalLightComponent(Math::Vector3 dir, Math::Vector3 color, float intensity) :
			direction(dir),
			color(color),
			intensity(intensity) {}

		Math::Vector3 direction;
		Math::Vector3 color;
		float intensity;
	};

	class PointLightComponent : public ECS::Component
	{
	public:
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

	class InputComponent : public ECS::Component 
	{
		InputComponent() {}
 
	};
}