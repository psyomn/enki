/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ENKI_WINDOW_H
#define _ENKI_WINDOW_H

#include <SDL2/SDL.h>

/**
 * Encapsulates most of the tedious window information for a game.
 */
struct enki_window {
	char *title;
	size_t width;
	size_t height;
	SDL_Window *window;
	SDL_Renderer *renderer;
	double fps;
};

/**
 * enki_window_new - will return a pointer to the window struct, or NULL on
 * error.  This will perform the SDL init.
 */
struct enki_window *enki_window_new(const char *title, size_t title_len,
				    size_t width, size_t height);


#endif
