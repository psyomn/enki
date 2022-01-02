/**
 * Copyright (c) 2021-2022, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdio.h>

#include "enki/common.h"
#include "enki/graphics.h"
#include "enki/level.h"
#include "enki/tilemap.h"
#include "enki/window.h"

#define ASSET_FILE "./assets/test_32x32.png"

static struct enki_tilemap* create_map(void)
{
	struct enki_window *window = enki_window_new(__FILE__, sizeof(__FILE__), 100, 100);
	if (window == NULL) goto error;

	struct enki_texture *texture = enki_texture_load_or_die(ASSET_FILE, window);
	if (texture == NULL) goto cleanup_window;

	struct enki_tilemap *tm = enki_tilemap_new(texture, 32, 32);
	if (tm == NULL) goto cleanup_texture;


	return tm;

cleanup_texture:
	enki_texture_free(texture);
cleanup_window:
	enki_window_free(window);
error:
	return NULL;
}

static int test_xy_to_pos_square(const struct enki_tilemap *tm)
{
	/* TODO: this might belong on the function header.
	 *
	 * For a grid like this:
	 *
	 *  +---+---+---+
	 *  | 0 | 1 | 2 |
	 *  +---+---+---+
	 *  | 3 | 4 | 5 |
	 *  +---+---+---+
	 *  | 6 | 7 | 8 |
	 *  +---+---+---+
	 *
	 * where width is 3 * 32, height 3 * 32,
	 * the ids of the tiles should give hints on which cordinates they
	 * reside in.  For the above, these are the translations.
	 *
	 * - 0 -> 0x0
	 * - 1 -> 32x0
	 * - 2 -> 64x0
	 * - 4 -> 32x32
	 * - 9 -> 64x64
	 */

	struct testcases {
		uint16_t id;
		size_t x;
		size_t y;
		size_t expect_x;
		size_t expect_y;
	} tcs[] = {
		{.id = 0, .x = 0, .y = 0, .expect_x = 0,  .expect_y = 0},
		{.id = 4, .x = 0, .y = 0, .expect_x = 32, .expect_y = 32},
		{.id = 7, .x = 0, .y = 0, .expect_x = 32, .expect_y = 64},
		{.id = 8, .x = 0, .y = 0, .expect_x = 64, .expect_y = 64},
	};

	for (size_t i = 0; i < ARRAY_SIZE(tcs); ++i) {
		enki_tilemap_id_to_xy(tm, tcs[i].id, &tcs[i].x, &tcs[i].y);
		if (tcs[i].x != tcs[i].expect_x ||
		    tcs[i].y != tcs[i].expect_y) {
			printf("[%lu] %s: expected {id:%d}{%lu,%lu} but got {%lu,%lu}\n",
			       i,
			       __func__,
			       tcs[i].id,
			       tcs[i].expect_x, tcs[i].expect_y,
			       tcs[i].x, tcs[i].y);
			return 1;
		}
	}

	return 0;
}

int main(void)
{
	struct enki_tilemap *tm = create_map();
	if (tm == NULL) {
		printf("error creating map\n");
		goto error;
	}

	const int ret =
		test_xy_to_pos_square(tm);

	enki_tilemap_free(tm);

	return ret;
error:
	return 1;
}


