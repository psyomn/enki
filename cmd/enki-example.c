/**
 * Copyright (c) 2021-2022, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>

#include "enki/core.h"
#include "enki/common.h"
#include "enki/graphics.h"
#include "enki/level.h"
#include "enki/object.h"
#include "enki/window.h"
#include "enki/tilemap.h"

struct enki_tilemap *create_map(struct enki_texture *texture)
{
	return enki_tilemap_new(texture, 32, 32);
}

struct enki_level *create_level(void)
{
	uint16_t tiles[] = {
		1,  1,  1,  1,  1,  1,  1,  1,  1, 1,
		1, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		1, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		1, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		1, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		1, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		1, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		1, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		1, 16, 16, 16, 16, 16, 16, 16, 16, 1,
		1,  1,  1,  1,  1,  1,  1,  1,  1, 1,
	};

	struct enki_level *level = enki_level_new(10, 10);

	enki_level_append_tiles(level,
				&tiles[0],
				ARRAY_SIZE(tiles));

	return level;
}

int main(void)
{
	if (enki_init() != ENKI_STATUS_OK) {
		fprintf(stderr, "problem initializing sdl2\n");
		return -1;
	}

	const size_t window_width = 320;
	const size_t window_height = 320;
	const char *window_title = "the game";
	const size_t window_title_len = sizeof(window_title);
	struct enki_window *win = enki_window_new(window_title,
						  window_title_len,
						  window_width,
						  window_height);

	struct enki_texture *summer_texture = enki_texture_load_or_die(
		"assets/Summer_Ground_32x32.png", win);

	struct enki_texture *autumn_texture = enki_texture_load_or_die(
		"assets/Autumn_Ground_32x32.png", win);

	struct enki_object *summer_tile = enki_object_new(0, 0, 32, 32,
							  summer_texture);
	if (summer_tile == NULL) {
		fprintf(stderr, "error creating summer tile object\n");
		return -1;
	}
	enki_object_set_col(summer_tile, 0, 0);

	struct enki_object *autumn_tile = enki_object_new(32, 0, 32, 32,
							  autumn_texture);
	if (autumn_tile == NULL) {
		fprintf(stderr, "error creating autumn tile texture\n");
		return -1;
	}
	enki_object_set_col(autumn_tile, 32, 0);

	struct enki_object *obj_list[] = {
		summer_tile,
		autumn_tile,
	};

	/* create some tilemap maker */
	struct enki_tilemap *tilemap = create_map(summer_texture);
	struct enki_level *level = create_level();

	enki_render(win,
		    level,
		    tilemap,
		    obj_list,
		    ARRAY_SIZE(obj_list));

	/* Cleanup */
	enki_level_free(level);
	enki_texture_free(summer_texture);
	enki_texture_free(autumn_texture);

	return 0;
}
