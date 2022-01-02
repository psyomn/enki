/**
 * Copyright (c) 2021, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ENKI_STATE_H
#define _ENKI_STATE_H

#include "enki/level.h"
#include "enki/object.h"
#include "enki/tilemap.h"

struct enki_state {
	size_t actors_len;
	struct enki_object **actors;
};
#endif
