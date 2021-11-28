/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "graphics.h"

#include <SDL2/SDL_image.h>

struct enki_texture *enki_texture_load(const char *path,
				       struct enki_window *win)
{
	struct enki_texture *tx = calloc(1, sizeof(*tx));
	if (tx == NULL) {
		perror("calloc");
		return NULL;
	}

	SDL_Surface *image = IMG_Load(path);
	if (image == NULL) {
		fprintf(stderr, "could not find texture '%s'\n", path);
		goto cleanup_tx;
	}

	tx->sdl_texture = SDL_CreateTextureFromSurface(win->renderer, image);
	if (tx->sdl_texture == NULL) {
		fprintf(stderr, "could not create texture\n");
		goto cleanup_all;
	}

	tx->width = image->w;
	tx->height = image->h;

	SDL_FreeSurface(image);
	return tx;

cleanup_all:
	SDL_FreeSurface(image);
cleanup_tx:
	free(tx);
	printf("BLARGH\n");
	return NULL;
}


struct enki_texture *enki_texture_load_or_die(const char *path,
					      struct enki_window *win)
{
	struct enki_texture *tx = enki_texture_load(path, win);
	if (tx == NULL) {
		fprintf(stderr, "could not find texture '%s'\n", path);
		abort();
	}
	return tx;
}

void enki_texture_free(struct enki_texture *texture)
{
	SDL_DestroyTexture(texture->sdl_texture);
	free(texture);
}

