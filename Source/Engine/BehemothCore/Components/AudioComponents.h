#pragma once
#include "ECS/Component.h"
#include "Audio/SoundClip.h"

namespace Behemoth
{
	struct AudioComponent : ECS::Component
	{
		AudioComponent() : 
			audioFilePath(""),
			soundClip(),
			isLooping(false),
			playSoundClip(true),
			stopSoundClip(false),
			isPlaying(false),
			destroyEntityOnFinished(true) 
		{}
												
		AudioComponent(std::string filepath, bool looping = false, bool destroyOnComplete = true, bool play = true) :
			audioFilePath(filepath), 
			isLooping(looping), 
			soundClip(filepath, looping), 
			playSoundClip(play), 
			stopSoundClip(false), 
			isPlaying(false), 
			destroyEntityOnFinished(destroyOnComplete) {}
												
		// Callback for when audio has finished playing
		std::function<void()> onAudioFinished;
		SoundClip soundClip;
		std::string	audioFilePath;
		bool isLooping;
		bool playSoundClip;
		bool stopSoundClip;
		bool isPlaying;
		bool destroyEntityOnFinished;
	};
}	