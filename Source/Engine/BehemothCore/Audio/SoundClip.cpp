#include "pch.h"
#include "SoundClip.h"
#include "Application/ResourceManager.h"
#include "NextAPI/App/app.h"

namespace Behemoth
{
	SoundClip::SoundClip() : audioFilepath(""), isLooping(false) {}

	SoundClip::SoundClip(std::string path, bool looping) : isLooping(looping), audioFilepath(path) {}

	void SoundClip::PlaySoundClip()
	{
		const std::string& p = ResourceManager::GetInstance().GetSoundClipPath(audioFilepath);
		App::PlaySound(p.c_str(), isLooping);
	}

	void SoundClip::StopSoundClip()
	{
		const std::string& p = ResourceManager::GetInstance().GetSoundClipPath(audioFilepath);
		App::StopSound(p.c_str());
	}

	bool SoundClip::SoundClipIsPlaying()
	{
		const std::string& p = ResourceManager::GetInstance().GetSoundClipPath(audioFilepath);
		return App::IsSoundPlaying(p.c_str());
	}
}