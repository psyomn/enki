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


void enki_render(struct enki_window *win,
		 struct enki_tilemap *tilemap,
		 struct enki_object **objlist,
		 size_t objlen)
{
	// TODO: this should just be one render loop (eventually)
	const double fragment = enki_calculate_frame_fragment(60.0);

	SDL_Event e = {0};
	while (1) {
		const double frame_start = SDL_GetPerformanceCounter();

		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT) return;

		for (size_t i = 0; i < objlen; ++i) {
			SDL_RenderCopy(win->renderer,
				       objlist[i]->texture->sdl_texture,
				       &objlist[i]->tx_pos,
				       &objlist[i]->collision);
		}

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
