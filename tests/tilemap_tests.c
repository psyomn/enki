#include <stdio.h>

#include "common.h"
#include "graphics.h"
#include "tilemap.h"
#include "window.h"

#define ASSET_FILE "./assets/Autumn_Ground_32x32.png"

static struct enki_tilemap* create_map(void)
{
	struct enki_window *window = enki_window_new(__FILE__, sizeof(__FILE__), 100, 100);
	if (window == NULL) goto error;

	struct enki_texture *texture = enki_texture_load_or_die(ASSET_FILE, window);
	if (texture == NULL) goto cleanup_window;

	struct enki_tilemap *tm = enki_tilemap_new(texture, 32, 32, 3);
	if (tm == NULL) goto cleanup_texture;

	uint16_t layer_1[] = {
		1, 1, 1,
		1, 0, 1,
		1, 1, 1,
	};

	uint16_t layer_2[] = {
		0, 0, 0,
		0, 1, 0,
		0, 0, 0,
	};

	uint16_t layer_3[] = {
		1, 0, 1,
		0, 1, 0,
		1, 0, 1,
	};

	memcpy(tm->tiles, layer_1, sizeof(layer_1));

	memcpy(tm->tiles + ARRAY_SIZE(layer_1),
	       layer_2, sizeof(layer_2));

	memcpy(tm->tiles + ARRAY_SIZE(layer_1) + ARRAY_SIZE(layer_2),
	       layer_3, sizeof(layer_3));

	return tm;

cleanup_texture:
	enki_texture_free(texture);
cleanup_window:
	enki_window_free(window);
error:
	return NULL;
}

static int test_first_row(const struct enki_tilemap *tm)
{
	return
		!((enki_tilemap_at(tm, 0, 0, 0) == 1) &&
		  (enki_tilemap_at(tm, 1, 0, 0) == 1) &&
		  (enki_tilemap_at(tm, 2, 0, 0) == 1) &&

		  (enki_tilemap_at(tm, 0, 0, 1) == 0) &&
		  (enki_tilemap_at(tm, 1, 0, 1) == 0) &&
		  (enki_tilemap_at(tm, 1, 1, 1) == 1) &&

		  (enki_tilemap_at(tm, 0, 0, 2) == 1) &&
		  (enki_tilemap_at(tm, 1, 0, 2) == 0) &&
		  (enki_tilemap_at(tm, 1, 1, 2) == 1));
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
	 *  | 6 | 7 | 9 |
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
		{.id = 0, .x = 0, .y = 0, .expect_x = 0, .expect_y = 0},
		{.id = 4, .x = 0, .y = 0, .expect_x = 1, .expect_y = 1},
		{.id = 7, .x = 0, .y = 0, .expect_x = 1, .expect_y = 2},
		{.id = 9, .x = 0, .y = 0, .expect_x = 2, .expect_y = 2},
	};

	for (size_t i = 0; i < ARRAY_SIZE(tcs); ++i) {
		enki_tilemap_id_to_xy(tm, 0, &tcs[i].x, &tcs[i].y);
		if (tcs[i].x != tcs[i].expect_x ||
		    tcs[i].y != tcs[i].expect_y) {
			printf("[%lu] %s: expected {id:%d}{%lu,%lu} but got {%lu,%lu}\n",
			       i,
			       __func__,
			       tcs->id,
			       tcs[i].x, tcs[i].y,
			       tcs[i].expect_x, tcs[i].expect_y);
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
		test_first_row(tm) |
		test_xy_to_pos_square(tm) ;

	enki_tilemap_free(tm);

	return ret;
error:
	return 1;
}
