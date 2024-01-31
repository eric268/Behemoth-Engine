#pragma once

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	struct CameraFactory
	{
		static ECS::EntityHandle CreateCamera(ECS::Registry& registry, bool isMain, const std::string& name = "Camera");
	};
}

