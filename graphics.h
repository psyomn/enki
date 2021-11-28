/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ENKI_GRAPHICS_H
#define _ENKI_GRAPHICS_H

#include <SDL2/SDL.h>

#include "window.h"

/***
 * TODO: decide if I want this: width and height are accessible through
 * SDL_QueryTexture, but it would be extra pain to need to call that instead of
 * storing useful properties like this.
 */
struct enki_texture {
	SDL_Texture *sdl_texture;
	size_t width;
	size_t height;
};

/**
 * enki_texture_load - load a texture using SDL, and store it in a custom struct
 * for later use.  Return NULL if path is invalid.
 */
struct enki_texture *enki_texture_load(const char *path,
				       struct enki_window *win);

/**
 * enki_texture_load_or_die - try to load the texture, or abort.
 */
struct enki_texture *enki_texture_load_or_die(const char *path,
					      struct enki_window *win);

/**
 * enki_texture_free - free the SDL texture resource, as well as the
 * enki_texture.
 */
void enki_texture_free(struct enki_texture *texture);

#endif
