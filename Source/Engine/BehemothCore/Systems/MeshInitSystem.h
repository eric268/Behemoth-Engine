#pragma once

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	class Mesh;
	struct MeshComponent;

	class MeshInitSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void InitMesh(Mesh& mesh);
		void InitSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& entityHandle);
	};

}