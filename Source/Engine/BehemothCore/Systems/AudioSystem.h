#pragma once
#include "ECS/Entity.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class AudioComponent;

	class AudioSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void StartAudio(AudioComponent* audioComponent);
		void StopAudio(AudioComponent* audioComponent);
		void OnFinished(ECS::Registry& registry, AudioComponent* audioComponent, const ECS::EntityHandle& handle);
	};
}

