# Copyright (c) 2021, Simon Symeonidis
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
project(
  'enki', 'c',
  version : '0.0.0',
  license : 'BSD 3.0'
)

dep_sdl2 = dependency('SDL2')
dep_sdl2_image = dependency('SDL2_image')
dep_sdl2_mixer = dependency('SDL2_mixer')
dep_threads = dependency('threads')
dep_m = meson.get_compiler('c').find_library('m', required : true)

dep_all = [
  dep_sdl2,
  dep_sdl2_image,
  dep_sdl2_mixer,
  dep_threads,
  dep_m,
]

c_flags = [
  '-std=c11',
  '-pedantic',
  '-march=native',
  '-Wall',
  '-Wextra',
  '-Werror',
  '-Wcast-align',
  '-Wwrite-strings',
  '-Wformat=2',
  '-Wswitch-default',
  '-Wswitch-enum',
  '-Wcast-align',
  '-Wpointer-arith',
  '-Wbad-function-cast',
  '-Wstrict-overflow=5',
  '-Wstrict-prototypes',
  '-Winline',
  '-Wundef',
  '-Wnested-externs',
  '-Wcast-qual',
  '-Wshadow',
  '-Wlogical-op',
  '-Wfloat-equal',
  '-Wredundant-decls',
  '-Wold-style-definition',
  '-fno-omit-frame-pointer',
  '-ffloat-store',
  '-fno-common',
  '-fstrict-aliasing',
  '-ggdb3',
  '-g',
  '-pipe',
  '-O2'
]

# libenki

enki_srcs = [
  'enki/core.c',
  'enki/graphics.c',
  'enki/level.c',
  'enki/object.c',
  'enki/window.c',
  'enki/tilemap.c',
]

enki_hdrs = [
  'enki/core.h',
  'enki/common.h',
  'enki/graphics.h',
  'enki/level.h',
  'enki/object.h',
  'enki/window.h',
  'enki/tilemap.h',
]

enki_inc = include_directories('enki')

enki_lib = shared_library(
  'enki',
  sources : enki_srcs,
  include_directories : enki_inc,
  dependencies : dep_all,
)

assets_dir = 'assets'
custom_target('copy script',
  input : assets_dir,
  output : assets_dir,
  command : ['cp', '-r', '@INPUT@', '@OUTPUT@'],
  install : false,
  build_by_default : true)

## command utils

executable('enki-example',
  sources : ['cmd/enki-example.c'],
  c_args : c_flags,
  link_with : enki_lib,
  dependencies : dep_all,
)

executable('enki-texture-info',
  sources : ['cmd/enki-texture-info.c'],
  c_args : c_flags,
  link_with : enki_lib,
  dependencies : dep_all,
)

## tests

tilemap_tests = executable(
  'tilemap_tests',
  'tests/tilemap_tests.c',
  link_with : enki_lib,
  dependencies : dep_all,
)

level_tests = executable(
  'level_tests',
  'tests/level_tests.c',
  link_with : enki_lib,
  dependencies : dep_all,
)

test('tilemap_tests', tilemap_tests, args : [])
test('level_tests', level_tests, args : [])

