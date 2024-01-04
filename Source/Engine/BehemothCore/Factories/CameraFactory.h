#pragma once
#include "ECS/Entity.h"
#include "ECS/Registry.h"

// stl
#include <string>

namespace Behemoth
{
	class CameraFactory
	{
	public:
		CameraFactory() = default;
		ECS::EntityHandle CreateCamera(ECS::Registry& registry, bool isMain, const std::string& name = "Camera");

	private:
	};
}

