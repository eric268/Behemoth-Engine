#include "ECS/Component.h"

namespace Behemoth
{
	class MeshComponent : public ECS::Component
	{
	public:
		MeshComponent() = default;
		MeshComponent(const std::string& modelName, const std::string& textureName, bool visible = true, bool drawWireMesh = false) : 
		modelFileName(modelName), 
			textureFileName(textureName), 
			mesh(modelName, textureName),
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
			FOV(90.0f),
			nearClippingPlane(0.1f),
			farClippingPlane(1000.0f),
			windowWidth(0.0f),
			windowHeight(0.0f),
			isMain(main),
			isDirty(true)
			{}

		~CameraComponent() = default;

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
		TransformComponent() : transformMatrix(Math::Matrix4x4::Identity()), dirty(true) {}
		Math::Matrix4x4 transformMatrix;
		Math::Vector3 position;
		bool dirty;
	};

	class MovementComponent : public ECS::Component
	{
	public:
		MovementComponent() : location(Math::Vector3{}) {}
		MovementComponent(Math::Vector3 vec) : location(vec) {}
		~MovementComponent() override {};

		Math::Vector3 location;
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

	class FrustrumComponent : public ECS::Component 
	{
	public:
		FrustrumComponent() = default;
		Math::Plane worldSpacePlanes[6];
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

}