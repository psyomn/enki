/**
 * copyright (c) 2021, simon symeonidis
 * all rights reserved.
 *
 * this source code is licensed under the bsd-style license found in the
 * license file in the root directory of this source tree.
 */

#ifndef _ENKI_TILEMAP_H
#define _ENKI_TILEMAP_H

#include <SDL2/SDL.h>

struct enki_tilemap_layer {
	uint16_t *tiles;
};

struct enki_tilemap {
	struct enki_texture *texture; /* shared */
	size_t width;
	size_t height;
	size_t layers_len;
	uint16_t tiles[];
};

/**
 * enki_tilemap_new - will create a new tilemap given the dimensions.
 */
struct enki_tilemap *enki_tilemap_new(struct enki_texture *texture,
				      size_t width, size_t height,
				      size_t num_layers);

/**
 * enki_tilemap_free - will free the tilemap.
 */
void enki_tilemap_free(struct enki_tilemap *tilemap);

/**
 * enki_tilemap_index_to_rect - provided the texture, and the index, update the
 * rec to point to that part of the texture.
 */
void enki_tilemap_index_to_rect(struct enki_tilemap *tm,
				uint16_t tile_index,
				SDL_Rect *rect);
#endif
