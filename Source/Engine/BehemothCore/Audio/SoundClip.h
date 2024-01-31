#pragma once

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

	private:
		std::string audioFilepath;
		bool isLooping;
	};
}

