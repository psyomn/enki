/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>

#include "core.h"
#include "common.h"
#include "graphics.h"
#include "object.h"
#include "window.h"
#include "tilemap.h"

struct enki_tilemap *create_map(struct enki_texture *texture) {
	struct enki_tilemap *tm = enki_tilemap_new(texture, 10, 10, 2);

	uint16_t layer_1[] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	uint16_t layer_2[] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	memcpy(tm->tiles, layer_1, sizeof(layer_1));

	memcpy(tm->tiles + ARRAY_SIZE(layer_1),
	       layer_2, sizeof(layer_2));

	return tm;
}

int main(void)
{
	if (enki_init() != ENKI_STATUS_OK) {
		fprintf(stderr, "problem initializing sdl2\n");
		return -1;
	}

	const size_t
		window_width = 640,
		window_height = 640;

	struct enki_window *win =
		enki_window_new("the", 4, window_width, window_height);

	struct enki_texture *summer_texture = enki_texture_load_or_die(
		"assets/Summer_Ground_32x32.png", win);

	struct enki_texture *autumn_texture = enki_texture_load_or_die(
		"assets/Autumn_Ground_32x32.png", win);

	struct enki_object *summer_tile = enki_object_new(0, 0, 32, 32, summer_texture);
	if (summer_tile == NULL) {
		fprintf(stderr, "error creating summer tile object\n");
		return -1;
	}
	enki_object_set_col(summer_tile, 0, 0);


	struct enki_object *autumn_tile = enki_object_new(32, 0, 32, 32, autumn_texture);
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

	enki_render(win,
		    tilemap,
		    obj_list,
		    ARRAY_SIZE(obj_list));

	/* C l e a n u p */
	enki_texture_free(summer_texture);
	enki_texture_free(autumn_texture);

	return 0;
}
