#include "enki/core.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "level.h"

enum enki_status enki_init(void)
{
	/* This will init the following:
	 *
	 *   SDL_INIT_TIMER: timer subsystem
	 *   SDL_INIT_AUDIO: audio subsystem
	 *   SDL_INIT_VIDEO: video subsystem; automatically initializes the events subsystem
	 *   SDL_INIT_JOYSTICK: joystick subsystem; automatically initializes the events subsystem
	 *   SDL_INIT_HAPTIC: haptic (force feedback) subsystem
	 *   SDL_INIT_GAMECONTROLLER: controller subsystem; automatically initializes the joystick subsystem
	 *   SDL_INIT_EVENTS: events subsystem
	 */
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return ENKI_STATUS_SDL_INIT_ERROR;
	}

	if (IMG_Init(IMG_INIT_PNG) == -1) {
		return EXIT_STATUS_SDL_IMG_INIT_ERROR;
	}

	return ENKI_STATUS_OK;
}

static void render_objects(SDL_Renderer *rr,
			   struct enki_object **objects,
			   size_t len)
{
	for (size_t i = 0; i < len; ++i) {
		if (objects[i]->texture == NULL) continue;

		SDL_RenderCopy(rr,
			       objects[i]->texture->sdl_texture,
			       &objects[i]->tx_pos, &objects[i]->tx_pos);
	}
}


// TODO: combine tm + level
static void render_tilemap(SDL_Renderer *rr,
			   const struct enki_tilemap *tm,
			   const struct enki_level *level)
{
	if (tm == NULL) return;
	if (level == NULL) return;

	for (size_t i = 0; i <= level->max_h_index; ++i) {
		for (size_t j = 0; j <= level->max_w_index; ++j){
			size_t tm_x = 0;
			size_t tm_y = 0;
			const uint16_t t_id = enki_level_at(level, j, i, 0);
			enki_tilemap_id_to_xy(tm, t_id, &tm_x, &tm_y);

			SDL_Rect pos = {
				.x = tm_x, .y = tm_y,
				.w = tm->tile_width,
				.h = tm->tile_height,
			}, stretch = {
				.x = tm->tile_width * j,
				.y = tm->tile_height * i,
				.w = tm->tile_width,
				.h = tm->tile_height,
			};

			SDL_RenderCopy(rr,
				       tm->texture->sdl_texture,
				       &pos, &stretch);
		}
	}
}

static void objects_process_events(struct enki_object **objlist,
				   const size_t obj_len,
				   SDL_Event *e)
{
	for (size_t i = 0; i < obj_len; ++i)
		if (objlist[i]->ehook) objlist[i]->ehook(objlist[i], e);
}

static void objects_process_renders(struct enki_object **objlist,
				    const size_t obj_len,
				    SDL_Renderer *rr)
{
	for (size_t i = 0; i < obj_len; ++i)
		if (objlist[i]->rhook) objlist[i]->rhook(objlist[i], rr);
}

static void objects_process_physics(struct enki_object **objlist,
				    const size_t obj_len,
				    const double dt)
{
	for (size_t i = 0; i < obj_len; ++i)
		if (objlist[i]->phook) objlist[i]->phook(objlist[i], dt);
}

static void objects_process_collisions(struct enki_object **objlist,
				       const size_t obj_len)
{
	for (size_t i = 0; i < obj_len; ++i) {
		for (size_t j = i + 1; j < obj_len; ++j) {
			const uint64_t in_groups = objlist[i]->group & objlist[j]->group;

			SDL_Rect result = {0};

			const SDL_bool rect_collision =
				SDL_IntersectRect(&objlist[i]->collision,
						  &objlist[j]->collision,
						  &result);

			if (rect_collision) printf("group:%lu sdlcol:%d\n hook:%p",
						   in_groups, rect_collision, objlist[i]->chook);

			if (rect_collision && in_groups) {
				if (objlist[i]->chook)
					objlist[i]->chook(objlist[i], objlist[j]);
				if (objlist[j]->chook)
					objlist[j]->chook(objlist[j], objlist[i]);
			}
		}
	}
}

void enki_render(struct enki_window *win,
		 struct enki_level *level,
		 struct enki_tilemap *tilemap,
		 struct enki_object **objlist,
		 size_t objlen)
{
	const double fragment = enki_calculate_frame_fragment(60.0);

	// TODO: this should be refactored to just be for one render frame
	SDL_Event e = {0};
	Uint64 frame_last = 0;
	Uint64 frame_now = SDL_GetPerformanceCounter();
	while (1) {
		frame_last = frame_now;

		// events
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) return;
			objects_process_events(objlist, objlen, &e);
		}

		// render
		SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 0xff);
		SDL_RenderClear(win->renderer);

		render_tilemap(win->renderer, tilemap, level);
		render_objects(win->renderer, objlist, objlen);
		// TODO: before or after precedence might be important
		objects_process_renders(objlist, objlen, win->renderer);
		SDL_RenderPresent(win->renderer);


		// FPS
		frame_now = SDL_GetPerformanceCounter();
		const double dt = (double)((frame_now - frame_last)/1000.0 ) /
					   (double) SDL_GetPerformanceFrequency();
		const double delay = enki_calculate_fps_delay(fragment, dt);

		// physics
		objects_process_physics(objlist, objlen, dt);
		objects_process_collisions(objlist, objlen);


		SDL_Delay(delay);
	}
}

double enki_calculate_frame_fragment(const double max_fps)
{
	return 1.0 / max_fps * 1000;
}

double enki_calculate_fps_delay(const double fragment, const double dt)
{
	return fragment - dt;
}
