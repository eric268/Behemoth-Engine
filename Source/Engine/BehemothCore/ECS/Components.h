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
		CameraComponent() : 
			viewMatrix(Math::Matrix4x4::Identity()), 
			FOV(0.0f),
			nearClippingPlane(0.1f), 
			farClippingPlane(1000.0f), 
			windowWidth(0.0f), 
			windowHeight(0.0f),
			isMain(false) {}
		~CameraComponent() = default;

		Math::Matrix4x4 viewMatrix;
		float FOV;
		float nearClippingPlane;
		float farClippingPlane;
		float windowWidth;
		float windowHeight;
		bool isMain;
	};

}