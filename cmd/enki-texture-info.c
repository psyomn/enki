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

struct session {
	SDL_Rect cursor;
	int visible;
	int tile_width;
	int tile_height;
} current_session = {
	.cursor = {0},
	.visible = 0,
	.tile_width = 0,
	.tile_height = 0,
};

struct enki_texture *texture = NULL;

static void cursor_render(struct enki_object *self, SDL_Renderer *rr)
{
	(void) self;

	if (!current_session.visible) return;

	SDL_SetRenderDrawColor(rr, 0xff, 0, 0, 0xff);
	SDL_RenderDrawRect(rr, &current_session.cursor);
}

void on_click(struct enki_object *self, SDL_Event *e)
{
	(void) self;

	if (e->type != SDL_MOUSEBUTTONDOWN) return;

	if (e->button.button == SDL_BUTTON_RIGHT) {
		current_session.visible = 0;
		return;
	}

	if (e->button.button != SDL_BUTTON_LEFT) return;

	current_session.visible = 1;

	Sint32 x = e->button.x;
	Sint32 y = e->button.y;

	current_session.cursor.x = (x / 32) * 32;
	current_session.cursor.y = (y / 32) * 32;
	current_session.cursor.w = current_session.tile_width;
	current_session.cursor.h = current_session.tile_height;

	int texture_width = 0, texture_height = 0;
	SDL_QueryTexture(texture->sdl_texture, NULL, NULL,
			 &texture_width, &texture_height);

	const int max_w_index = texture_width / 32;

	printf("%d %d = id(%d)\n",
	       current_session.cursor.x,
	       current_session.cursor.y,
	       current_session.cursor.y/32 * max_w_index +
	       current_session.cursor.x/32
	);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: \n"
		       "  %s <tileset_32x32.png>", argv[0]);
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

	texture = enki_texture_load_or_die(texture_filename, win);

	// TODO: tile width should be inferred through name
	current_session.tile_width = 32;
	current_session.tile_height = 32;
	struct enki_tilemap *tm = enki_tilemap_new(texture, 32, 32);

	struct enki_object *object = enki_object_new(0, 0,
						     tm->max_w_index * tm->tile_width,
						     tm->max_h_index * tm->tile_height,
						     texture);

	struct enki_object *cursor = enki_object_new(0, 0, 0, 0, NULL);
	enki_object_set_rhook(object, cursor_render);

	enki_object_set_col(object,
			    tm->max_w_index * tm->tile_width,
			    tm->max_h_index * tm->tile_height);

	if (enki_object_add_ehook(object, on_click) == -1) {
		fprintf(stderr, "%s: could not add hook to object\n", __FILE__);
		abort();
	}

	struct enki_object *obj_list[] = {
		object,
		cursor,
	};

	enki_tilemap_print(tm);

	enki_render(win, NULL, NULL, obj_list, ARRAY_SIZE(obj_list));

	for (size_t i = 0; i < ARRAY_SIZE(obj_list); ++i)
		enki_object_free(obj_list[i]);

	enki_tilemap_free(tm);
	enki_texture_free(texture);
	enki_window_free(win);

	return EXIT_SUCCESS;
}
