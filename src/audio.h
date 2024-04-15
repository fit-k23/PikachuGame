#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef PIKACHUGAME_AUDIO_H
#define PIKACHUGAME_AUDIO_H

#define MINIAUDIO_IMPLEMENTATION
#define MA_ENABLE_ONLY_SPECIFIC_BACKENDS
#define MA_ENABLE_WASAPI
#define MA_ENABLE_WINMM
#define MA_NO_FLAC
#include "miniaudio.h"

// gcc -c -O2 miniaudio.c -o miniaudio -DMINIAUDIO_IMPLEMENTATION -DMA_ENABLE_ONLY_SPECIFIC_BACKENDS

#include <random>
#include <string>
#include <thread>

#endif

const int MAX_ALBUM_SIZE = 10;

struct Sound{
	string filePath;
	ma_sound *sound = nullptr;
};

struct SoundAlbum{
	Sound sounds[MAX_ALBUM_SIZE];
	int size = 0;
	bool loop = false;
	bool randomize = false;
	int current = -1;
	bool isEmpty() const;
	void shuffleSound();
	Sound* nextSound();
	bool addSoundFromFilePath(ma_engine &engine, const string& filePath, bool preLoad);
};

Sound *SoundAlbum::nextSound() {
	if (size == 0) return nullptr;
	if (current == size - 1) {
		if (!loop) {
			return nullptr;
		} else {
			if (randomize) {
				shuffleSound();
			}
			current = 0;
		}
	} else {
		current++;
	}
	return &sounds[current];
}

bool SoundAlbum::isEmpty() const {
	return size == 0;
}

void SoundAlbum::shuffleSound() {
	if (size < 1) return;
	shuffle(sounds, sounds + size, std::mt19937(std::random_device()())); //Random shuffle algo
}

bool SoundAlbum::addSoundFromFilePath(ma_engine &engine, const string &filePath, bool preLoad = false) {
	if (size == MAX_ALBUM_SIZE - 1) {
		return false;
	}
	if (!preLoad) {
		if (fileExist(filePath.c_str())) {
			sounds[size++].filePath = filePath;
			return true;
		}
		return false;
	}

	if (ma_sound_init_from_file(&engine, filePath.c_str(), MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, sounds[size].sound) == MA_SUCCESS) {
		size++;
		return true;
	}
	return false;
}

struct AudioEngine{
	ma_engine engine{};
	SoundAlbum* album = nullptr;
	bool FLAG_PLAY_NEXT_SOUND = false;
	bool FLAG_STOP_PLAY = false;
	bool FLAG_SWITCH_ALBUM = false;
	float volume = 1.0;
	bool isPlaying = false;

	bool init();
	void uninit();

	void setVolume(float v);
};

bool AudioEngine::init() {
	ma_result result;
	result = ma_engine_init(nullptr, &engine);
	if (result != MA_SUCCESS) {
		return false;
	}
	ma_engine_set_volume(&engine, volume);
	return true;
}

void AudioEngine::uninit() {
	ma_engine_uninit(&engine);
}

void AudioEngine::setVolume(float v) {
	volume = v;
	ma_engine_set_volume(&engine, volume);
}

ma_sound* initSoundFromFile(AudioEngine *engine, const string &filename) {
	ma_sound *sound;
	ma_sound_init_from_file(&engine->engine, filename.c_str(),MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, sound);
	return sound;
}

bool playEngine(AudioEngine *engine) {
	if (engine->album->size == 0) return false;
	engine->isPlaying = true;
	Sound *current = engine->album->nextSound();
	float volume = engine->volume;
	ma_sound* sound;
	while (current != nullptr) {
		sound = current->sound;
		if (sound == nullptr) {
			sound = initSoundFromFile(engine, current->filePath);
			if (sound == nullptr) {
				current = engine->album->nextSound();
				continue;
			}
		}
		ma_sound_set_fade_in_milliseconds(sound, 0, 1, 500); // Fade in 0.5 second before the sound start
		if (ma_sound_start(sound) != MA_SUCCESS) {
			if (current->sound == nullptr) {
				// deallocate the sound if it was generated if it was loaded from storage
				ma_sound_uninit(sound);
			}
			continue;
		}
		ma_float len;
		ma_sound_get_length_in_seconds(sound, &len);
		ma_sound_stop_with_fade_in_milliseconds(sound, (len - 0.5) * 1000); //Fade out 1 second before sound end | NOLINT(*-narrowing-conversions)

		while (!ma_sound_get_at_end(sound)) {
			if (engine->FLAG_SWITCH_ALBUM || engine->FLAG_STOP_PLAY || engine->FLAG_PLAY_NEXT_SOUND) {
				ma_sound_stop(sound);
				break;
			}
			if (volume != engine->volume) {
				volume = engine->volume;
				ma_engine_set_volume(&engine->engine, volume);
			}
			Sleep(100);
		}
		if (current->sound == nullptr) {
			// deallocate the sound if it was generated if it was loaded from storage
			ma_sound_uninit(sound);
		} else {
			//Reset sound index to 0 so the sound will play from the start in later!
			ma_sound_seek_to_pcm_frame(sound, 0);
		}
		if (engine->FLAG_PLAY_NEXT_SOUND) {
			engine->FLAG_PLAY_NEXT_SOUND = false;
		}
		if (engine->FLAG_STOP_PLAY) {
			engine->isPlaying = false;
			break;
		}
		current = engine->album->nextSound();
	}
	engine->isPlaying = false;
	return true;
}
//
//ma_sound *AudioEngine::nextSound() {
//	if (size == 0) return nullptr;
//	if (current == size - 1) {
//		if (!loop) {
//			isPlaying = false;
//			return nullptr;
//		} else {
//			current = 0;
//		}
//	} else {
//		current++;
//	}
//	return &sounds[current];
//}
//
//ma_sound *playSoundFromFile(AudioEngine *engine, const char *filename, float volume = 1.0) {
//	ma_sound *sound;
//	ma_sound_init_from_file(&engine->engine, filename,
//							MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, sound);
//	ma_sound_set_volume(sound, volume);
//	ma_sound_start(sound);
//	// Local value escape the prison :c
//	return sound;
//}
#pragma clang diagnostic pop