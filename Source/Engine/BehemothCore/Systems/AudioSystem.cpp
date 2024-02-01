#include "pch.h"
#include "AudioSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/AudioComponents.h"

namespace Behemoth
{
	void AudioSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entityHandle, audioFileComp] : registry.Get<AudioComponent>())
		{
			// Sound clip has stopped
			if (!audioFileComp->soundClip.SoundClipIsPlaying())
			{
				// We have stopped it ourselves, continue
				if (audioFileComp->stopSoundClip)
				{
					continue;
				}
				// audio file has finished playing
				else if (audioFileComp->isPlaying)
				{
					OnFinished(registry, audioFileComp, entityHandle);
				}
				// Audio file has not started playing so play it
				else if (audioFileComp->playSoundClip)
				{
					StartAudio(audioFileComp);
				}
			}
			else if (audioFileComp->stopSoundClip)
			{
				// Sound clip is playing but we want to stop it
				StopAudio(audioFileComp);
			}
		}
	}

	void AudioSystem::StartAudio(AudioComponent* audioComp)
	{
		audioComp->soundClip.PlaySoundClip();
		audioComp->isPlaying = true;
	}

	void AudioSystem::StopAudio(AudioComponent* audioComp)
	{
		audioComp->soundClip.StopSoundClip();
		audioComp->isPlaying = false;
		audioComp->playSoundClip = false;
	}

	void AudioSystem::OnFinished(ECS::Registry& registry, AudioComponent* audioComp, const ECS::EntityHandle& entityHandle)
	{
		if (audioComp->onAudioFinished)
		{
			audioComp->onAudioFinished();
		}

		if (audioComp->destroyEntityOnFinished)
		{
			registry.DestroyEntity(ECS::EntityHandle(entityHandle));
		}
	}
}