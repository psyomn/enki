/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "common.h"

#include "window.h"

struct enki_window *enki_window_new(const char *title, size_t title_len,
				    size_t width, size_t height)
{
    struct enki_window *win = calloc(1, sizeof(*win));
    win->width = width;
    win->height = height;

    win->window = SDL_CreateWindow(win->title,
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   win->width,
				   win->height,
				   SDL_WINDOW_SHOWN);
    if (win->window == NULL) {
	perror("sdl2");
	goto cleanup_exit;
    }

    win->renderer = SDL_CreateRenderer(
        win->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if (win->renderer == NULL) {
	    fprintf(stderr, "could not create renderer\n");
	    goto cleanup_window;
    }

    return win;
cleanup_window:
    SDL_DestroyWindow(win->window);
    free(win->title);
cleanup_exit:
    return NULL;
}

