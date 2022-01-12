/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <stdio.h>
#include <stddef.h>
#include <time.h>

#include "enki/core.h"
#include "enki/common.h"
#include "enki/window.h"
#include "enki/object.h"

const char *PONG_TEXTURE_FILE = "assets/PongBasicBoard.png";
const int PLAYER_ID = 1;
const int ENEMY_ID = 2;
const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 320;

struct ai_paddle_state {
	struct enki_object *player_paddle;
	struct enki_object *ball;
} ai_paddle_state = {0};

struct game_state {
	int score_player;
	int score_ai;
} game_state = {0};

struct enki_level *create_map(void)
{
	uint16_t tiles[] = {
		 0,  1,  1,  1,  2,  2,  1,  1,  1,  4,
		 5,  6,  6,  6,  7,  7,  6,  6,  6,  9,
		 5,  6,  6,  6,  7,  7,  6,  6,  6,  9,
		 5,  6,  6,  6,  7,  7,  6,  6,  6,  9,
		 5,  6,  6,  6,  7,  7,  6,  6,  6,  9,
		 5,  6,  6,  6,  7,  7,  6,  6,  6,  9,
		 5,  6,  6,  6,  7,  7,  6,  6,  6,  9,
		 5,  6,  6,  6,  7,  7,  6,  6,  6,  9,
		 5,  6,  6,  6,  7,  7,  6,  6,  6,  9,
		20, 21, 21, 21, 22, 22, 21, 21, 21, 24,
	};

	struct enki_level *level = enki_level_new(10, 10);
	enki_level_append_tiles(level, &tiles[0], ARRAY_SIZE(tiles));

	return level;
}

static void paddle_render(struct enki_object *self, SDL_Renderer *rr)
{
	SDL_SetRenderDrawColor(rr, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawRect(rr, &self->tx_pos);

	// SDL_SetRenderDrawColor(rr, 0xff, 0x00, 0x00, 0xff);
	// SDL_RenderDrawRect(rr, &self->collision);
}

static void ball_render(struct enki_object *self, SDL_Renderer *rr)
{
	SDL_SetRenderDrawColor(rr, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderDrawRect(rr, &self->tx_pos);

	// SDL_SetRenderDrawColor(rr, 0xff, 0x00, 0x00, 0xff);
	// SDL_RenderDrawRect(rr, &self->collision);
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
	enki_object_update_pos(self, dt);

	if (self->y < 0)
		enki_object_set_y(self, 0.0);
	if (self->y + self->tx_pos.h > WINDOW_HEIGHT)
		enki_object_set_y(self, WINDOW_HEIGHT - self->tx_pos.h);
}

static void paddle_physics_ai(struct enki_object *self, const double dt)
{
	const double speed = 200000.0;
	const struct ai_paddle_state *state = (struct ai_paddle_state *)self->extra;
	const struct enki_object *ball = state->ball;

	if (ball->tx_pos.y > self->tx_pos.y)
		self->speed_y = speed;
	else if (ball->tx_pos.y < self->tx_pos.y)
		self->speed_y = -speed;
	else
		self->speed_y = 0;

	if (self->y < 0)
		enki_object_set_y(self, 0.0);
	if (self->y + self->tx_pos.h > WINDOW_HEIGHT)
		enki_object_set_y(self, WINDOW_HEIGHT - self->tx_pos.h);

	enki_object_update_pos(self, dt);
}

static void ball_physics(struct enki_object *self, const double dt)
{
	enki_object_update_pos(self, dt);

	// TODO: extract something like "outside window view?"
	if (self->tx_pos.x > WINDOW_WIDTH + self->tx_pos.w) {
		++game_state.score_player;
		goto place_ball_on_center;
	} else if (self->tx_pos.x < -self->tx_pos.w) {
		++game_state.score_ai;
		goto place_ball_on_center;
	}

	if (self->tx_pos.y < 0 || self->tx_pos.y > WINDOW_HEIGHT - self->tx_pos.h)
		self->speed_y = -self->speed_y;
	return;

place_ball_on_center:
	printf("player:%d ai:%d\n", game_state.score_player, game_state.score_ai);
	const int init_speed = 64000;
	self->speed_x = rand() % 1000 < 500 ? init_speed : -init_speed;
	self->speed_y = rand() % 1000 < 500 ? init_speed : -init_speed;
	enki_object_set_x(self, WINDOW_WIDTH / 2);
	enki_object_set_y(self, WINDOW_HEIGHT / 2);
}

static void ball_on_collision(struct enki_object *self, struct enki_object *other)
{
	const double incr_speed = 10000.0;
	const double next_speed =
		self->speed_x > 0 ?
		self->speed_x + incr_speed :
		self->speed_x - incr_speed;

	if (other->id == PLAYER_ID && self->speed_x < 0)
		self->speed_x = -next_speed;
	if (other->id == ENEMY_ID && self->speed_x > 0)
		self->speed_x = -next_speed;
}

int main(void)
{
	srand(time(NULL));

	const char *title = "pong";
	struct enki_window *win = enki_window_new(title, sizeof(title),
						  WINDOW_WIDTH, WINDOW_HEIGHT);
	// backgrounds
	struct enki_texture *background = enki_texture_load_or_die(PONG_TEXTURE_FILE, win);
	struct enki_tilemap *tilemap = enki_tilemap_new(background, 32, 32);
	struct enki_level *map = create_map();

	// actors
	struct enki_object *paddle_1 = enki_object_new(32, 32, 16, 64, NULL);
	enki_object_set_rhook(paddle_1, paddle_render);
	enki_object_set_ehook(paddle_1, paddle_event);
	enki_object_set_phook(paddle_1, paddle_physics);
	enki_object_set_col(paddle_1, 16, 64);
	paddle_1->id = PLAYER_ID;
	paddle_1->group = 1;

	struct enki_object *paddle_2 = enki_object_new(WINDOW_WIDTH - 64 + 16, 32, 16, 64, NULL);
	enki_object_set_rhook(paddle_2, paddle_render);
	enki_object_set_phook(paddle_2, paddle_physics_ai);
	enki_object_set_col(paddle_2, 16, 64);
	paddle_2->id = ENEMY_ID;
	paddle_2->group = 1;
	enki_object_update_pos(paddle_2, 0.0); // TODO: set hitboxes properly and delete this

	struct enki_object *ball = enki_object_new(WINDOW_WIDTH / 2 - 4,
						   WINDOW_HEIGHT / 2 - 4,
						   8, 8, NULL);
	enki_object_set_col(ball, 8, 8);
	enki_object_set_rhook(ball, ball_render);
	enki_object_set_phook(ball, ball_physics);
	enki_object_set_chook(ball, ball_on_collision);

	// TODO: this should be a "place ball routine somewhere"
	const int init_speed = 64000;
	ball->speed_x = rand() % 1000 < 500 ? init_speed : -init_speed;
	ball->speed_y = rand() % 1000 < 500 ? init_speed : -init_speed;
	ball->group = 1;

	ai_paddle_state.player_paddle = paddle_1;
	ai_paddle_state.ball = ball;
	paddle_2->extra = (struct ai_paddle_state*)&ai_paddle_state;

	struct enki_object *actors[] = {
		paddle_1,
		paddle_2,
		ball,
	};

	enki_render(win, map, tilemap, actors, ARRAY_SIZE(actors));

	// cleanup
	enki_level_free(map);
	enki_tilemap_free(tilemap);
	enki_texture_free(background);
	enki_window_free(win);
	for (size_t i = 0; i < ARRAY_SIZE(actors); ++i)
		enki_object_free(actors[i]);

	return 0;
}
