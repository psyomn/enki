/**
 * Copyright (c) 2021-2022, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "level.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct enki_level *enki_level_new(const size_t x, const size_t y)
{
	const size_t len = x * y;
	const size_t tile_bytes = len * sizeof(uint16_t);
	const size_t total_bytes = sizeof(struct enki_level) + tile_bytes;
	struct enki_level *level = calloc(1, total_bytes);
	if (level == NULL) goto error;

	level->max_w_index = x - 1;
	level->max_h_index = y - 1;
	level->len = len;

	return level;
error:
	return NULL;
}

void enki_level_free(struct enki_level *level)
{
	free(level);
}

uint16_t enki_level_at(const struct enki_level *level,
		       size_t x, size_t y, size_t layer)
{
	(void) layer;

	const size_t location = (y*(level->max_w_index+1)) + x;
	assert(location < level->len);
	return level->tiles[location];
}

void enki_level_append_tiles(struct enki_level *level,
			     const uint16_t *tiles,
			     size_t tile_len)
{
	if (tile_len != (1 + level->max_h_index) * (1 + level->max_w_index)) {
		fprintf(stderr, "wrong amount of tiles to append");
		abort();
	}

	memcpy(level->tiles + level->cursor, tiles,
	       tile_len * sizeof(tiles[0]));

	level->cursor += tile_len;
}

void enki_level_debug(struct enki_level *level)
{
	for (size_t i = 0; i < level->len; ++i) {
		printf("%d ", level->tiles[i]);
		if (i % (level->max_w_index + 1) == 0) printf("\n");
	}
}
