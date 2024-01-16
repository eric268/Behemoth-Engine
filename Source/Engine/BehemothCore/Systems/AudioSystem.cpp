#include "pch.h"
#include "AudioSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/AudioComponents.h"

namespace Behemoth
{
	void AudioSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entity, audioFileComp] : registry.Get<AudioComponent>())
		{
			// Sound clip has stopped
			if (!SoundClip::SoundClipIsPlaying(audioFileComp->audioFilePath))
			{
				// We have stopped it ourselves so continue
				if (audioFileComp->stopSoundClip)
				{
					continue;
				}
				// audio file has finished playing
				else if (audioFileComp->isPlaying)
				{
					if (audioFileComp->onAudioFinished)
					{
						audioFileComp->onAudioFinished();
					}

					if (audioFileComp->destroyEntityOnFinished)
					{
						registry.DestroyEntity(ECS::EntityHandle(entity));
					}
				}
				// Audio file has not started playing so play it
				else if (audioFileComp->playSoundClip)
				{
					audioFileComp->soundClip.PlaySoundClip();
					audioFileComp->isPlaying = true;
				}
			}
			else
			{
				// Sound clip is playing but we want to stop it
				if (audioFileComp->stopSoundClip)
				{
					audioFileComp->soundClip.StopSoundClip();
					audioFileComp->isPlaying = false;
				}
			}
		}
	}
}