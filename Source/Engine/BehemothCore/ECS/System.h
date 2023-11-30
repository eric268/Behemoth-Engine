#pragma once
#include "Registry.h"

namespace ECS
{
	class System
	{
	public:
		virtual ~System() = default;

		virtual void Run(Registry& registry) = 0;
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem() = default;

		virtual void Run(Registry& registry) override;
	};

	class ModelLoadingSystem : public System
	{
	public:
		ModelLoadingSystem() = default;

		virtual void Run(Registry& registry) override;
	};

	class CameraSystem : public System
	{
	public: 
		CameraSystem() = default;
		virtual void Run(Registry& registry) override;

		void UpdateViewMatrix(CameraComponent& component);
	};
}
