/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <stdio.h>
#include <stddef.h>

#include "enki/core.h"
#include "enki/common.h"
#include "enki/window.h"
#include "enki/object.h"

static void paddle_render(struct enki_object *self, SDL_Renderer *rr)
{
	SDL_SetRenderDrawColor(rr, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawRect(rr, &self->tx_pos);
}

static void render_ball(struct enki_object *self, SDL_Renderer *rr)
{
	SDL_SetRenderDrawColor(rr, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawRect(rr, &self->tx_pos);
}

static void paddle_event(struct enki_object *self, SDL_Event *e)
{
	const int speed = 300000;

	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_UP)
			self->speed_y = -speed;
		if (e->key.keysym.sym == SDLK_DOWN)
			self->speed_y = speed;

		return;
	}

	if (e->type == SDL_KEYUP) {
		if (e->key.keysym.sym == SDLK_UP || e->key.keysym.sym == SDLK_DOWN)
			self->speed_y = 0;

		return;
	}
}

static void paddle_physics(struct enki_object *self, const double dt)
{
	self->tx_pos.y += dt * (double) self->speed_y;
	// printf("dist(%f) speed(%d) pos.y(%d)\n",
	//        (dt * (double) self->speed_y),
	//        self->speed_y, self->tx_pos.y);
}

int main(void)
{
	const size_t
		window_width = 320,
		window_height = 320;

	const char *title = "pong";
	struct enki_window *win = enki_window_new(title, sizeof(title),
						  window_width, window_height);

	struct enki_object *paddle_1 = enki_object_new(32, 32, 16, 96, NULL);
	enki_object_set_rhook(paddle_1, paddle_render);
	enki_object_add_ehook(paddle_1, paddle_event);
	enki_object_set_phook(paddle_1, paddle_physics);
	enki_object_set_col(paddle_1, 16, 96);

	struct enki_object *paddle_2 = enki_object_new(window_width - 64 + 16, 32, 16, 96, NULL);
	enki_object_set_rhook(paddle_2, paddle_render);
	enki_object_set_col(paddle_1, 16, 96);

	struct enki_object *ball = enki_object_new(window_width / 2 - 4,
						   window_height / 2 - 4,
						   8, 8, NULL);
	enki_object_set_rhook(ball, render_ball);

	struct enki_object *actors[] = {
		paddle_1,
		paddle_2,
		ball,
	};

	enki_render(win, NULL, NULL, actors, ARRAY_SIZE(actors));

	// cleanup
	enki_window_free(win);
	for (size_t i = 0; i < ARRAY_SIZE(actors); ++i)
		enki_object_free(actors[i]);

	return 0;
}
