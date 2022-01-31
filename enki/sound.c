#include "enki/sound.h"

int enki_init_sound(void)
{
	return
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) |
		Mix_Init(MIX_INIT_OGG | MIX_INIT_MOD);
}

struct enki_sound *enki_sound_new(const char *path)
{
	Mix_Chunk *tmp = Mix_LoadWAV(path);
	printf(">> %p %s \n", tmp, path);
	if (tmp == NULL) goto error;

	struct enki_sound *ret = calloc(1, sizeof(*ret));
	ret->chunk = tmp;
	return ret;
error:
	return NULL;
}

struct enki_sound *enki_sound_load_or_die(const char *path)
{
	struct enki_sound *sound = enki_sound_new(path);
	if (sound == NULL) {
		fprintf(stderr, "could not load soundfile: %s\n", path);
		abort();
	}
	return sound;
}

void enki_sound_free(struct enki_sound *sound)
{
	Mix_FreeChunk(sound->chunk);
	free(sound);
}

void enki_sound_play(const struct enki_sound *sound)
{
	Mix_PlayChannel(-1, sound->chunk, 0);
}
