#include "core.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

void render_tilemap(SDL_Renderer *rr, struct enki_tilemap *tm)
{
	const size_t length = tm->len;

	SDL_Rect pos = {
		.x = 0, .y = 0,
		.w = tm->tile_width, .h = tm->tile_height,
	},
	stretch = {
		.x = 0, .y = 0,
		.w = tm->tile_width, .h = tm->tile_height,
	};

	for (size_t l = 0; l < tm->layers_len; ++l) {
		for (size_t i = 0; i < tm->tile_height; ++i) {
			for (size_t j = 0; j < tm->tile_width; ++j){
				uint16_t t_data = enki_tilemap_at(tm, j, i, l);

				SDL_RenderCopy(rr,
					       tm->texture->sdl_texture,
					       &pos, &stretch);
			}
		}
	}
}

void enki_render(struct enki_window *win,
		 struct enki_tilemap *tilemap,
		 struct enki_object **objlist,
		 /* render_fn_t *(*render_fns)(SDL_Renderer *), */
		 size_t objlen)
{
	const double fragment = enki_calculate_frame_fragment(60.0);
	// TODO: this should just be one render loop (eventually)
	SDL_Event e = {0};
	while (1) {
		const double frame_start = SDL_GetPerformanceCounter();

		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT) return;

		render_tilemap(win->renderer, tilemap);

		// for (size_t i = 0; i < objlen; ++i) {
		// 	SDL_RenderCopy(win->renderer,
		// 		       objlist[i]->texture->sdl_texture,
		// 		       &objlist[i]->tx_pos,
		// 		       &objlist[i]->collision);
		// }

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
