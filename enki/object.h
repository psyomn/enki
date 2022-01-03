/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ENKI_OBJECT_H
#define _ENKI_OBJECT_H

#include <SDL2/SDL.h>

#include "enki/graphics.h"

struct enki_object {
	/** contains rendering position information */
	SDL_Rect tx_pos;

	/** contains position/collision bounds. */
	SDL_Rect collision;

	/** Shared.  Can be NULL. */
	struct enki_texture *texture;

	/** SDL_Event hooks. */
	void (**ehooks)(SDL_Event*);

	/** ehook len */
	size_t ehook_len;
};

/**
 * enki_object_set_col - set the collision box/position for the object.
 */
void enki_object_set_col(struct enki_object *object, int xpos, int ypos);

/**
 * enki_object_new - will create a new object, possible to render in the scene.
 */
struct enki_object *enki_object_new(int xpos,
				    int ypos,
			            int width,
			            int height,
			            struct enki_texture *texture);

/**
 * enki_object_free - will free the game object
 */
void enki_object_free(struct enki_object *object);

/**
 * enki_object_add_ehook - adds a hook, to be executed upon an event.
 *
 * return 0 on success, return 1 on error (usually failure on realloc).
 */
int enki_object_add_ehook(struct enki_object *object,
			  void (*ehook)(SDL_Event*));
#endif
