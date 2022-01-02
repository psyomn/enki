/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ENKI_CORE_H
#define _ENKI_CORE_H

#include "enki/common.h"
#include "enki/level.h"
#include "enki/object.h"
#include "enki/window.h"
#include "enki/tilemap.h"

/**
 * enki_init - will initialize SDL specific stuff.
 */
enum enki_status enki_init(void);

/**
 * enki_render - will render graphics, depending on what objects exist.
 */
void enki_render(struct enki_window *win,
		 struct enki_level *level,
		 struct enki_tilemap *tilemap,
		 struct enki_object **objlist,
		 size_t objlen);

/**
 * enki_calculate_frame_fragment - will calculate the time slice that allows for
 * frame renders.
 */
double enki_calculate_frame_fragment(double max_fps);

/**
 * enki_calculate_fps_delay - is the delay amount to regulate fps in a loop.
 * max_fps is the desired frames in a second.
 */
double enki_calculate_fps_delay(double fragment,
				uint64_t start_frame,
				uint64_t end_frame);

#endif
