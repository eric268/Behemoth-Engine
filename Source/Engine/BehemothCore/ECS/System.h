#pragma once
#include "Registry.h"
#include "Render/Primitives.h"

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
	private:

		bool ClipBackFace(const Math::Vector3& cameraLocation, const Math::Vector3 primitiveVerts[3]);
	};

	class ModelInitalizeSystem : public System
	{
	public:
		ModelInitalizeSystem() = default;

		virtual void Run(Registry& registry) override;
	};

	class CameraSystem : public System
	{
	public: 
		CameraSystem() = default;
		virtual void Run(Registry& registry) override;

		void UpdatePerspectiveMatrix(CameraComponent& component);
	};

	class RotationSystem : public System
	{
	public:
		RotationSystem() = default;

		virtual void Run(Registry& registry) override;
	};
}
