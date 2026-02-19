#include "systems/sound_handler.h"

/// Loads a sound sample from file
/// @param fname Path to audio file
/// @param looped If true, the sample loops when played
/// @return Sample pointer, or nullptr on failure
void* SoundHandler::load_sound(const char* fname, bool looped) {
	int flags = 0;
	if (looped) flags |= FSOUND_LOOP_NORMAL;
	return FSOUND_Sample_Load(FSOUND_FREE, fname, flags, 0, 0);
}

/// Sets volume for a playing channel handle
/// @param handle Channel handle returned by StartSound/PlayMusic
/// @param volume Volume in [0,1] (clamped)
void SoundHandler::change_volume(int handle, float volume) {
	if (handle <= 0) return;
	if (volume <= 0) volume = 0;
	if (volume > 1) volume = 1;
	FSOUND_SetVolume(handle, (int)(volume * 255));
}

/// Starts playing a loaded sound sample
/// @param sound Sample pointer returned by LoadSound
/// @param volume Volume in [0,1] (clamped)
/// @return Channel handle, or -1 on error
int SoundHandler::play_sound(void* sound, float volume) {
	if (!sound) return -1;
	if (volume <= 0) volume = 0;
	if (volume > 1) volume = 1;
	int chan = FSOUND_PlaySound(FSOUND_FREE, (FSOUND_SAMPLE*)sound);
	FSOUND_SetVolume(chan, (int)(volume * 255));
	return chan;
}

/// Stops a playing channel
/// @param handle Channel handle returned by StartSound/PlayMusic
void SoundHandler::stop_sound(int handle) {
	if (handle <= 0) return;
	FSOUND_StopSound(handle);
}

/// Starts looping background music from file (stops previous music if any)
/// @param fname Path to audio file
/// @param volume Volume in [0,1] (clamped)
/// @return Channel handle, or -1 on error
int SoundHandler::play_music(const char* fname, float volume) {
	if (music_) stop_music();
	music_ = FSOUND_Stream_Open(fname, FSOUND_LOOP_NORMAL, 0, 0);
	int chan = FSOUND_Stream_Play(FSOUND_FREE, music_);
	if (volume <= 0) volume = 0;
	if (volume > 1) volume = 1;
	FSOUND_SetVolume(chan, (int)(volume * 255));
	return chan;
}

/// Stops and releases currently playing music stream (if any)
void SoundHandler::stop_music() {
	if (music_) {
		FSOUND_Stream_Close(music_);
	}
	music_ = nullptr;
}