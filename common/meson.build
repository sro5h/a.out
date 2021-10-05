common_sources = [
  'byte_order.c',
  'console.c',
  'dummy.c',
  'log.c',
  'messages.c',
  'stream.c',
  'util.c'
]

common_public_headers = include_directories(
  '../external'
)

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
  c_args: build_args
)
