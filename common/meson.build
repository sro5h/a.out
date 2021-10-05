common_sources = [
  'byte_order.c',
  'console.c',
  'dummy.c',
  'messages.c',
  'stream.c',
  'util.c'
]

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
  c_args: build_args
)
