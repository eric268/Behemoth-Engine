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
		static bool SoundClipIsPlaying(const std::string& path);

		std::string audioFilepath;
		bool isLooping;
	private:
		std::string fullFilepath;
	};
}

