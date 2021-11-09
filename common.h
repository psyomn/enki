/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ENKI_COMMON_H
#define _ENKI_COMMON_H

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

enum enki_status {
	ENKI_STATUS_OK = 0,
	ENKI_STATUS_SDL_INIT_ERROR,
	EXIT_STATUS_SDL_IMG_INIT_ERROR,
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#endif
