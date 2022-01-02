/**
 * Copyright (c) 2021-2022, Simon Symeonidis
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "enki/level.h"
#include "enki/common.h"

struct enki_level *create_level()
{
	struct enki_level *level = enki_level_new(3, 3);

	const uint16_t tiles[] = {
		1, 1, 1,
		1, 0, 1,
		1, 1, 1,
	};

	enki_level_append_tiles(level, &tiles[0], ARRAY_SIZE(tiles));

	enki_level_debug(level);

	return level;
}

static int test_level_at(struct enki_level *level)
{
	struct {
		size_t x;
		size_t y;
		size_t l;
		uint16_t actual_id;
		uint16_t expected_id;
	} test_cases[] = {
		{.x = 0, .y = 0, .l = 0, .expected_id = 1},
		{.x = 1, .y = 0, .l = 0, .expected_id = 1},
		{.x = 2, .y = 0, .l = 0, .expected_id = 1},

		{.x = 0, .y = 1, .l = 0, .expected_id = 1},
		{.x = 1, .y = 1, .l = 0, .expected_id = 0},
		{.x = 2, .y = 1, .l = 0, .expected_id = 1},

		{.x = 0, .y = 2, .l = 0, .expected_id = 1},
		{.x = 0, .y = 2, .l = 0, .expected_id = 1},
		{.x = 0, .y = 2, .l = 0, .expected_id = 1},
	};

	for (size_t i = 0; i < ARRAY_SIZE(test_cases); ++i) {
		test_cases[i].actual_id = enki_level_at(level,
							test_cases[i].x,
							test_cases[i].y,
							test_cases[i].l);

		if (test_cases[i].expected_id != test_cases[i].actual_id) {
			printf("[%lu] %s: expected %d but got %d\n",
			       i,
			       __func__,
			       test_cases[i].expected_id,
			       test_cases[i].actual_id);
			return -1;
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	(void) argc; (void) argv;

	struct enki_level *level = create_level();
	const int ret = test_level_at(level);

	return ret;
}
