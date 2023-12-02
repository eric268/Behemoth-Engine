#pragma once

#include "Generator.h"
#include "Render/Mesh.h"
#include "Math/MathCore.h"

// stl
#include <iostream>

namespace ECS
{
	class Components
	{
	public:
		Components() = default;
		virtual ~Components() {}
	};

	class MeshComponent : public Components
	{
	public:
		MeshComponent() {}
		MeshComponent(const std::string& path) : filepath(path) {}
		~MeshComponent() override {};

		BehemothEngine::Mesh mesh;
		std::string filepath;
	};

	class HealthComponent : public Components
	{
	public:
		HealthComponent(int health) : currentHealth(health) {}
		~HealthComponent() override {};

		int currentHealth;
	};

	class MovementComponent : public Components
	{
	public:
		MovementComponent() {}
		~MovementComponent() override {};

		Math::Vector3 location;
	};

	class RotationComponent : public Components 
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

	class PhysicsComponent : public Components 
	{
	public:
		PhysicsComponent() {}
		~PhysicsComponent() override {};
	};

	class CameraComponent : public Components 
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
			isMain(main) {}
		~CameraComponent() = default;

		Math::Matrix4x4 viewMatrix;
		Math::Matrix4x4 perspectiveMatrix;
		float FOV;
		float nearClippingPlane;
		float farClippingPlane;
		float windowWidth;
		float windowHeight;
		bool isMain;
	};

	class TransformComponent : public Components
	{
	public:
		TransformComponent() : transformMatrix(Math::Matrix4x4::Identity()) {}
		Math::Matrix4x4 transformMatrix;
	};

	class MeshInitalizeComponent : public Components
	{
	public:
		MeshInitalizeComponent() = default;
	};

}