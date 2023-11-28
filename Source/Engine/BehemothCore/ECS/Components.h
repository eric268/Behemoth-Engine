#pragma once

#include "Generator.h"


class Components
{
public:
	Components() = default;
	virtual ~Components() {}
	using type = Components;
};

class RenderComponent : public Components 
{
public:
	RenderComponent() : typeID(Generator::Value<RenderComponent>())
	{

	}
	~RenderComponent() override {};

	using type = RenderComponent;

	int GetTypeID()
	{
		return typeID;
	}

private:
	int typeID;
};

class HealthComponent : public Components
{
public:
	HealthComponent(int health) : currentHealth{ health }, typeID(Generator::Value<HealthComponent>())
	{

	}
	~HealthComponent() override {};

	using type = HealthComponent;

	int currentHealth;

	int GetTypeID()
	{
		return typeID;
	}

private:
	int typeID;
};

class MovementComponent : public Components
{
public:
	MovementComponent() : typeID(Generator::Value<MovementComponent>())
	{

	}
	~MovementComponent() override {};

	int GetTypeID()
	{
		return typeID;
	}

private:
	int typeID;
};
