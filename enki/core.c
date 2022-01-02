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

void render_tilemap(SDL_Renderer *rr,
		    const struct enki_tilemap *tm,
		    const struct enki_level *level)
{
	// TODO layers
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

void enki_render(struct enki_window *win,
		 struct enki_level *level,
		 struct enki_tilemap *tilemap,
		 struct enki_object **objlist,
		 size_t objlen)
{
	const double fragment = enki_calculate_frame_fragment(60.0);

	// TODO: this should be refactored to just be for one render frame
	SDL_Event e = {0};
	while (1) {
		const double frame_start = SDL_GetPerformanceCounter();

		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT) return;

		render_tilemap(win->renderer, tilemap, level);

		SDL_RenderPresent(win->renderer);

		const double frame_end = SDL_GetPerformanceCounter();
		SDL_Delay(enki_calculate_fps_delay(fragment,
						   frame_start,
						   frame_end));
	}
}

double enki_calculate_frame_fragment(double max_fps)
{
	return 1.0 / max_fps;
}

double enki_calculate_fps_delay(double fragment,
				uint64_t start_frame,
				uint64_t end_frame)
{
	const double diff = end_frame - start_frame;
	const double elapsed = diff / SDL_GetPerformanceFrequency();
	return floor(fragment - elapsed);
}
