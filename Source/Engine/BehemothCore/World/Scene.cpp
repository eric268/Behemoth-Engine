#include "Scene.h"
#include "World.h"

#include <iostream>

namespace Behemoth
{
	Scene::Scene() : registry()
	{
	}

	void Scene::Init()
	{
		std::cout << "Init called in scene\n";
	}
	void Scene::Update(const float deltaTime)
	{
		std::cout << "Init called in update\n";
	}

	void Scene::Shutdown()
	{
		std::cout << "Init called in shutdown\n";
	}
}