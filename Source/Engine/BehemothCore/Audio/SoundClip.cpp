#include "pch.h"
#include "SoundClip.h"
#include "Application/ResourceManager.h"
#include "NextAPI/App/app.h"

namespace Behemoth
{
	SoundClip::SoundClip() : audioFilepath(""), isLooping(false) {}

	SoundClip::SoundClip(std::string path, bool looping) : isLooping(looping) 
	{
		audioFilepath = ResourceManager::GetInstance().GetSoundClipPath(path);
		App::StopSound(audioFilepath.c_str());
	}

	void SoundClip::PlaySoundClip()
	{
		App::PlaySound(audioFilepath.c_str(), isLooping);
	}

	void SoundClip::StopSoundClip()
	{
		App::StopSound(audioFilepath.c_str());
	}

	bool SoundClip::SoundClipIsPlaying(const std::string& filepath)
	{
		return App::IsSoundPlaying(ResourceManager::GetInstance().GetSoundClipPath(filepath).c_str());
	}
}