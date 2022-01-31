#ifndef _ENKI_SOUND_H
#define _ENKI_SOUND_H

#include <SDL2/SDL_mixer.h>

struct enki_sound {
	Mix_Chunk *chunk;
};

/** need to call this for init if using sound */
int enki_init_sound(void);

/** path is to a wav file */
struct enki_sound *enki_sound_new(const char *path);

struct enki_sound *enki_sound_load_or_die(const char *path);

void enki_sound_free(struct enki_sound *sound);

void enki_sound_play(const struct enki_sound *sound);

#endif
