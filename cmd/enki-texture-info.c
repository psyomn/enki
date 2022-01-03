/**
 * copyright (c) 2021-2022, simon symeonidis
 * all rights reserved.
 *
 * this source code is licensed under the bsd-style license found in the
 * license file in the root directory of this source tree.
 */

#include <stdio.h>

#include "enki/core.h"
#include "enki/common.h"
#include "enki/graphics.h"
#include "enki/object.h"
#include "enki/tilemap.h"

#include <SDL2/SDL.h>

void on_click(SDL_Event *e)
{
	if (e->type != SDL_KEYDOWN) return;
	printf("yep\n");
}

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

	const char *window_title = "texture info";
	struct enki_window *win = enki_window_new(
		window_title, sizeof(window_title),
		window_width, window_height);

	struct enki_texture *texture = enki_texture_load_or_die(texture_filename, win);

	struct enki_tilemap *tm = enki_tilemap_new(texture, 32, 32);

	struct enki_object *object = enki_object_new(0, 0,
						     tm->max_w_index * tm->tile_width,
						     tm->max_h_index * tm->tile_height,
						     texture);
	enki_object_set_col(object,
			    tm->max_w_index * tm->tile_width,
			    tm->max_h_index * tm->tile_height);

	if (enki_object_add_ehook(object, on_click) == -1) {
		fprintf(stderr, "%s: could not add hook to object\n", __FILE__);
		abort();
	}

	struct enki_object *obj_list[] = {
		object,
	};

	enki_tilemap_print(tm);

	enki_render(win,
		    NULL,
		    NULL,
		    obj_list,
		    ARRAY_SIZE(obj_list));

	enki_object_free(object);
	enki_tilemap_free(tm);
	enki_texture_free(texture);
	enki_window_free(win);

	return EXIT_SUCCESS;
}
