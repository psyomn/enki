/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "object.h"

struct enki_object *enki_object_new(int xpos,
				    int ypos,
			            int width,
			            int height,
			            struct enki_texture *texture)
{
	struct enki_object *object = calloc(1, sizeof(*object));
	if (object == NULL) {
		perror("calloc");
		return NULL;
	}

	object->texture = texture;

	object->tx_pos.x = xpos;
	object->tx_pos.y = ypos;
	object->tx_pos.w = width;
	object->tx_pos.h = height;

	/* if these are not set, we segfault in SDL due to 0 dimensions. */
	object->collision.x = 0;
	object->collision.y = 0;
	object->collision.w = 32;
	object->collision.h = 32;

	return object;
}

void enki_object_set_col(struct enki_object *object, int xpos, int ypos)
{
	object->collision.x = xpos;
	object->collision.y = ypos;
}
