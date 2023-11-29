#pragma once

#include "Generator.h"

namespace ECS
{
	class Components
	{
	public:
		Components() = default;
		virtual ~Components() {}
	};

	class RenderComponent : public Components
	{
	public:
		RenderComponent() {}
		~RenderComponent() override {};
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

}