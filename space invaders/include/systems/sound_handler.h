#pragma once

#include "fmod/api/inc/fmod.h"

class SoundHandler {
private:
	FSOUND_STREAM* music_ = nullptr;
public:
	void* load_sound(const char* fname, bool looped = false);
	void change_volume(int handle, float volume);

	int play_sound(void* sound, float volume = 1);
	void stop_sound(int handle);

	int play_music(const char* fname, float volume = 1);
	void stop_music();
};