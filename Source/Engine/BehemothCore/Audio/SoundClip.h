#pragma once

#include <string>

namespace Behemoth
{
	class SoundClip
	{
	public:
		SoundClip();
		SoundClip(std::string path, bool looping);
		void PlaySoundClip();
		void StopSoundClip();
		bool SoundClipIsPlaying();

		std::string audioFilepath;
		bool isLooping;
	private:
	};
}

