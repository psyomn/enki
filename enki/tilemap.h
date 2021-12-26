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

#include "enki/graphics.h"

struct enki_tilemap_layer {
	uint16_t *tiles;
};

struct enki_tilemap {
	struct enki_texture *texture;	/* shared */
	size_t tile_width;
	size_t tile_height;
	size_t layers_len;
	size_t len;			/* total bytes len */
	size_t max_w_index;		/* texture_width divided by tile width */
	size_t max_h_index;		/* texture_height divided by tile height */
	uint16_t tiles[];
};

/**
 * enki_tilemap_new - will create a new tilemap given the dimensions.
 *
 * Returns a enki_tilemap pointer, or NULL on error.
 */
struct enki_tilemap *enki_tilemap_new(struct enki_texture *texture,
				      size_t width, size_t height,
				      size_t num_layers);

/**
 * enki_tilemap_free - will free the tilemap.
 */
void enki_tilemap_free(struct enki_tilemap *tilemap);

/**
 * enki_tilemap_at - given layer, and x, y coordinates, get that tile
 * id.  The ids are assigned as such, on a given picture/texture:
 *
 *   +---+---+---+
 *   | 1 | 1 | 1 |
 *   +---+---+---+
 *   | 1 | 0 | 1 |
 *   +---+---+---+
 *   | 1 | 1 | 1 |
 *   +---+---+---+
 */
uint16_t enki_tilemap_at(const struct enki_tilemap *tilemap,
			 size_t x, size_t y, size_t layer);

/**
 * enki_tilemap_id_to_xy - simple helper function that turns grid positions
 * into pixel positions.
 *
 * example: tilemap of 32x32 pixel sized tiles.
 *
 *   +---+---+---+
 *   | 0 | 1 | 2 |
 *   +---+---+---+
 *   | 3 | 4 | 5 |
 *   +---+---+---+
 *   | 6 | 7 | 8 |
 *   +---+---+---+
 */
void enki_tilemap_id_to_xy(const struct enki_tilemap *tilemap,
			   const uint16_t id, size_t *x, size_t *y);

#endif
