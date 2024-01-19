#pragma once

#include <string>

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	struct CameraFactory
	{
		ECS::EntityHandle CreateCamera(ECS::Registry& registry, bool isMain, const std::string& name = "Camera");
	};
}

