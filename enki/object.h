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
#endif
