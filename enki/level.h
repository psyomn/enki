/**
 * Copyright (c) 2021-2022, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ENKI_LEVEL_H
#define _ENKI_LEVEL_H

#include <stddef.h>
#include <stdint.h>

/**
 * enki_level is a scene in the game.
 */
struct enki_level {
	size_t max_w_index;
	size_t max_h_index;
	size_t len;
	size_t cursor;
	uint16_t tiles[];
};

/**
 * enki_level_new - create a new level.
 */
struct enki_level *enki_level_new(const size_t x, const size_t y);

/**
 * enki_level_free - destructor for enki_level data
 */
void enki_level_free(struct enki_level *level);

/**
 * enki_level_at - given layer, and x, y coordinates, get that tile
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
uint16_t enki_level_at(const struct enki_level *level,
		       size_t x, size_t y, size_t layer);

/**
 * enki_level_append_tiles - will copy the given tile information to the end of
 * the tile array.  Caller is responsible for correct amounts.
 */
void enki_level_append_tiles(struct enki_level *level,
			     const uint16_t *tiles,
			     size_t tile_len);

/**
 *  enki_level_debug - will print interesting information about the enki_level
 *  struct.
 */
void enki_level_debug(struct enki_level *level);
#endif
