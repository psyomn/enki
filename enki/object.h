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
	uint64_t id;

	/** contains rendering position information */
	SDL_Rect tx_pos;

	/** contains position/collision bounds. */
	SDL_Rect collision;

	/** Shared.  Can be NULL. */
	struct enki_texture *texture;

	/** SDL_Event hooks. */
	void (**ehooks)(struct enki_object *self, SDL_Event*);

	/** ehook len */
	size_t ehook_len;

	/** SDL_Renderer hooks. */
	void (*rhook)(struct enki_object *self, SDL_Renderer*);

	/** physics hook. */
	void (*phook)(struct enki_object *self, double dt);

	/** collision hook. */
	void (*chook)(struct enki_object *self, struct enki_object *other);

	double x;
	double y;
	double speed_x;
	double speed_y;

	uint64_t group;

	/** anything the user might want to attach to (eg a custom struct) */
	void *extra;
};

/**
 * enki_object_set_col - set the collision box/position for the object.
 */
void enki_object_set_col(struct enki_object *object, int xpos, int ypos);

/**
 * enki_object_new - will create a new object, possible to render in the scene.
 */
struct enki_object *enki_object_new(int xpos, int ypos,
			            int width, int height,
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
			  void (*ehook)(struct enki_object*, SDL_Event*));

/**
 * enki_object_set_rhook - sets a hook, to render special things only SDL2 would
 * understand.  This is so that you could technically override any behavior,
 * and, say, render an SDL_Rect for example.
 *
 * return 0 on success, return 1 on error (usually failure on realloc).
 */
void enki_object_set_rhook(struct enki_object *object,
			   void (*rhook)(struct enki_object*, SDL_Renderer*));

/**
 * enki_object_set_phook - sets a hook, to calculate the physics on a delta
 * time.
 */
void enki_object_set_phook(struct enki_object *object,
			   void (*phook)(struct enki_object*, double dt));

/**
 * enki_object_set_chook - sets a hook, to define behavior upon collision
 */
void enki_object_set_chook(struct enki_object *object,
			   void (*chook)(struct enki_object*, struct enki_object*));

/**
 * enki_object_update_pos - will update the x, y pos depending on set velocities
 * speed_x, and speed_y.
 */
void enki_object_update_pos(struct enki_object *object, double dt);

void enki_object_set_x(struct enki_object *object, double x);

void enki_object_set_y(struct enki_object *object, double y);

#endif
