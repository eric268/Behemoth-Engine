#pragma once
#include "ECS/Component.h"
#include "Audio/SoundClip.h"

#include <string>
#include <functional>

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
			destroyEntityOnFinished(true) {}

		AudioComponent(std::string filepath, bool destroyOnComplete = true, bool looping = false, bool play = true) :
			audioFilePath(filepath), 
			isLooping(looping), 
			soundClip(filepath, looping), 
			playSoundClip(play), 
			stopSoundClip(false), 
			isPlaying(false), 
			destroyEntityOnFinished(destroyOnComplete) {}
		
		std::function<void()> onAudioFinished;
		SoundClip soundClip;
		std::string audioFilePath;
		bool isLooping;
		bool playSoundClip;
		bool stopSoundClip;
		bool isPlaying;
		bool destroyEntityOnFinished;
	};
}