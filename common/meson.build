common_sources = [
  'dummy.c'
]

build_args = [
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_EXPORTS'
endif

common_target = library(
  'common',
  common_sources,
  c_args: build_args
)
