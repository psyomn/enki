/**
 * copyright (c) 2021, simon symeonidis
 * all rights reserved.
 *
 * this source code is licensed under the bsd-style license found in the
 * license file in the root directory of this source tree.
 */
#include "tilemap.h"

struct enki_tilemap *enki_tilemap_new(struct enki_texture *texture,
				      size_t width, size_t height,
				      size_t num_layers)
{
	const size_t tile_bytes =
		(width * height) * num_layers * sizeof(uint16_t);

	struct enki_tilemap *tm = calloc(1, sizeof(*tm) + tile_bytes);
	if (tm == NULL) {
		perror("calloc");
		goto error;
	}

	tm->texture = texture;
	tm->width = width;
	tm->height = height;
	tm->layers_len = num_layers;

	return tm;

error:
	return NULL;
}

void enki_tilemap_free(struct enki_tilemap *tilemap)
{
	free(tilemap);
}

void enki_tilemap_index_to_rect(struct enki_tilemap *tm,
				uint16_t tile_index,
				SDL_Rect *rect)
{
	// TODO
	return;
}

