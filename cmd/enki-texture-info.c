/**
 * copyright (c) 2021-2022, simon symeonidis
 * all rights reserved.
 *
 * this source code is licensed under the bsd-style license found in the
 * license file in the root directory of this source tree.
 */

#include <stdio.h>

#include "enki/graphics.h"
#include "enki/tilemap.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: ");
		return 1;
	}

	const char *texture_filename = argv[1];

	const size_t
		window_width = 640,
		window_height = 640;

	const char *window_title = "the game";
	struct enki_window *win = enki_window_new(
		window_title, sizeof(window_title),
		window_width, window_height);

	struct enki_texture *texture = enki_texture_load_or_die(
		texture_filename, win);

	struct enki_tilemap* tm = enki_tilemap_new(texture, 32, 32);

	printf(
		"tile width  : %lu \n"
		"tile height : %lu \n"
		"len         : %lu \n"
		"max x index : %lu \n"
		"max y index : %lu \n",
		tm->tile_width,
		tm->tile_height,
		tm->len,
		tm->max_w_index,
		tm->max_h_index
	);

	enki_tilemap_free(tm);
	enki_texture_free(texture);
	enki_window_free(win);

	return EXIT_SUCCESS;
}
