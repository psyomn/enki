/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "enki/object.h"

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
	object->x = xpos;
	object->tx_pos.y = ypos;
	object->y = ypos;
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
	object->collision.w = xpos;
	object->collision.h = ypos;
}

void enki_object_free(struct enki_object *object)
{
	free(object->ehooks);
	free(object);
}

int enki_object_add_ehook(struct enki_object *object,
			  void (*ehook)(struct enki_object*, SDL_Event*))
{
	void (**tmpehook)(struct enki_object*, SDL_Event*) =
		reallocarray(object->ehooks, object->ehook_len + 1,
			     sizeof(object->ehooks[0]));

	if (tmpehook == NULL) return -1;

	object->ehooks = tmpehook;

	object->ehooks[object->ehook_len] = ehook;
	object->ehook_len += 1;
	return 0;
}

void enki_object_set_rhook(struct enki_object *object,
			   void (*rhook)(struct enki_object*, SDL_Renderer*))
{
	object->rhook = rhook;
}

void enki_object_set_phook(struct enki_object *object,
			   void (*phook)(struct enki_object*, double dt))
{
	object->phook = phook;
}

void enki_object_set_chook(struct enki_object *object,
			   void (*chook)(struct enki_object*, struct enki_object*))
{
	object->chook = chook;
}

void enki_object_update_pos(struct enki_object *object, double dt)
{
	object->x += dt * object->speed_x;
	object->y += dt * object->speed_y;
	object->tx_pos.x = object->x;
	object->tx_pos.y = object->y;
	object->collision.x = object->x;
	object->collision.y = object->y;
}

void enki_object_set_x(struct enki_object *object, double x)
{
	object->x = x;
	object->tx_pos.x = x;
	object->collision.x = x;
}

void enki_object_set_y(struct enki_object *object, double y)
{
	object->y = y;
	object->tx_pos.y = y;
	object->collision.y = y;
}

