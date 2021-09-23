client_sources = [
  'main.c'
]

client_public_headers = include_directories(
  '..'
)

build_args = [
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_IMPORTS'
endif

enet = dependency('libenet')

client_target = executable(
  'client',
  client_sources,
  include_directories: client_public_headers,
  link_with: common_target,
  dependencies: enet
)
