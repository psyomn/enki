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

struct enki_tilemap {
	struct enki_texture *texture;	/* shared */
	size_t tile_width;
	size_t tile_height;
	size_t len;			/* total bytes len */
	size_t max_w_index;		/* texture_width divided by tile width */
	size_t max_h_index;		/* texture_height divided by tile height */
};

/**
 * enki_tilemap_new - will create a new tilemap given the dimensions.
 *
 * Returns a enki_tilemap pointer, or NULL on error.
 */
struct enki_tilemap *enki_tilemap_new(struct enki_texture *texture,
				      size_t width, size_t height);

/**
 * enki_tilemap_free - will free the tilemap.
 */
void enki_tilemap_free(struct enki_tilemap *tilemap);

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

/**
 * enki_tilemap_print - print tilemap information
 */
void enki_tilemap_print(const struct enki_tilemap *tilemap);
#endif
