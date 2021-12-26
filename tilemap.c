/**
 * copyright (c) 2021, simon symeonidis
 * all rights reserved.
 *
 * this source code is licensed under the bsd-style license found in the
 * license file in the root directory of this source tree.
 */
#include "tilemap.h"

#include <assert.h>

struct enki_tilemap *enki_tilemap_new(struct enki_texture *texture,
				      size_t width, size_t height,
				      size_t num_layers)
{
	// TODO: width and height should be infered from texture instead

	const size_t max_len = (width * height) * num_layers;
	const size_t tile_bytes = max_len * sizeof(uint16_t);

	struct enki_tilemap *tm = calloc(1, sizeof(*tm) + tile_bytes);
	if (tm == NULL) {
		perror("calloc");
		goto error;
	}

	tm->texture = texture;
	tm->tile_width = width;
	tm->tile_height = height;
	tm->layers_len = num_layers;
	tm->len = max_len;

	int texture_width = 0;
	int texture_height = 0;
	SDL_QueryTexture(tm->texture->sdl_texture,
			 NULL, NULL,
			 &texture_width, &texture_height);

	tm->max_w_index = texture_width / tm->tile_width;
	tm->max_h_index = texture_height / tm->tile_height;

	return tm;

error:
	return NULL;
}

void enki_tilemap_free(struct enki_tilemap *tilemap)
{
	free(tilemap);
}

uint16_t enki_tilemap_at(const struct enki_tilemap *tilemap,
			 size_t x, size_t y, size_t layer)
{
	const size_t location =
		/* layer offset */
		(layer * tilemap->tile_width * tilemap->tile_height) +
		/* grid offset */
		(y*tilemap->tile_width) + x;

	assert(location < tilemap->len);

	return tilemap->tiles[location];
}

void enki_tilemap_id_to_xy(const struct enki_tilemap *tilemap,
			   const uint16_t id, size_t *x, size_t *y)
{
	// TODO maybe optimize me
	assert(id < tilemap->max_h_index * tilemap->max_h_index);

	*x = (id % tilemap->max_w_index) * tilemap->tile_width;
	*y = (id / tilemap->max_h_index) * tilemap->tile_height;
}
