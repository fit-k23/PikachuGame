#ifndef PIKACHUGAME_AUDIO_H
#define PIKACHUGAME_AUDIO_H

#define MINIAUDIO_IMPLEMENTATION
#define MA_ENABLE_ONLY_SPECIFIC_BACKENDS
#define MA_ENABLE_WASAPI
#define MA_ENABLE_WINMM
#define MA_NO_FLAC

#include <string>
#include "miniaudio.h"
#include <thread>

#endif //PIKACHUGAME_AUDIO_H

const int MAX_SOUND = 30;

struct AudioEngine {
	ma_engine engine{};
	ma_sound sounds[MAX_SOUND] = {};
	int size = 0;
	bool loop = false;
	bool randomize = false;
	float volume = 1.0;
	int current = -1;

	int init();

	void uninit();

	bool addSoundFromFile(const std::string &fileName);

	void setVolume(float v);

	bool isPlaying = false;

	float getVolume();

	ma_sound *nextSound();
};

int AudioEngine::init() {
	ma_result result;
	result = ma_engine_init(nullptr, &engine);
	if (result != MA_SUCCESS) {
		return -1;
	}
	return 1;
}

ma_sound *AudioEngine::nextSound() {
	if (size == 0) return nullptr;
	if (current == size - 1) {
		if (!loop) {
			isPlaying = false;
			return nullptr;
		} else {
			current = 0;
		}
	} else {
		current++;
	}
	return &sounds[current];
}

void AudioEngine::uninit() {
	ma_engine_uninit(&engine);
	for (int i = 0; i < size; i++) {
		ma_sound_uninit(&sounds[i]);
	}
}

void AudioEngine::setVolume(float v) {
	volume = v;
}

float AudioEngine::getVolume() { // NOLINT(*-make-member-function-const)
	return volume;
}

bool AudioEngine::addSoundFromFile(const std::string &fileName) {
	if (size == MAX_SOUND - 1) {
//		cout << "Failed to add " << fileName << "\n";
		return false;
	} else {
//		cout << "Added " << fileName << "\n";
	}
	ma_result mr = ma_sound_init_from_file(&engine, fileName.c_str(), MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
										   nullptr, nullptr, &sounds[size++]);
	return mr == MA_SUCCESS;
}

ma_sound *playSound(AudioEngine *engine, const char *filename, float volume = 1.0) {
	ma_sound *sound;
	ma_sound_init_from_file(&engine->engine, filename, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr,
							sound);
	ma_sound_set_volume(sound, volume);
	ma_sound_start(sound);
	// Local value escape the prison :c
	return sound;
}

bool playLoop(AudioEngine *engine) {
	if (engine->size == 0) return false;
	engine->isPlaying = true;
	ma_sound *current = engine->nextSound();
	float volume = engine->volume;
	while (current != nullptr) {
		if (ma_sound_start(current) != MA_SUCCESS) {
			continue;
		}
		float len = 0;
		ma_sound_get_length_in_seconds(current, &len);
		while (!ma_sound_get_at_end(current)) {
			if (!engine->isPlaying) {
				ma_sound_stop(current);
			}
			if (volume != engine->volume) {
				volume = engine->volume;
				ma_engine_set_volume(&engine->engine, volume);
			}
			Sleep(100);
		}
		current = engine->nextSound();
	}
	engine->isPlaying = false;
	return true;
}