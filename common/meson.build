common_sources = [
  'byte_order.c',
  'console.c',
  'dummy.c',
  'host.c',
  'id_pool.c',
  'log.c',
  'messages.c',
  'movement.c',
  'ring.c',
  'state.c',
  'stream.c',
  'tick.c',
  'transform.c',
  'util.c',
  'vec2.c'
]

common_public_headers = include_directories(
  '../external'
)

common_dependencies = [
]

if build_machine.system() == 'linux'
  common_dependencies += meson.get_compiler('c').find_library('m')
  common_dependencies += dependency('threads')
endif

common_dependencies += dependency('libenet')
common_dependencies += dependency('chipmunk')

build_args = [
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_EXPORTS'
endif

if build_machine.system() == 'linux'
  build_args += '-D_DEFAULT_SOURCE'
endif

common_target = library(
  'common',
  common_sources,
  include_directories: common_public_headers,
  c_args: build_args,
  dependencies: common_dependencies
)
