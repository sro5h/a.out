common_sources = [
  'dummy.c',
  'console.c',
  'byte_order.c'
]

build_args = [
  '-D_DEFAULT_SOURCE' # TODO: Only define on linux
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_EXPORTS'
endif

common_target = library(
  'common',
  common_sources,
  c_args: build_args
)
