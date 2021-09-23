server_sources = [
  'main.c'
]

server_public_headers = include_directories(
  '..'
)

build_args = [
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_IMPORTS'
endif

enet = dependency('libenet')

server_target = executable(
  'server',
  server_sources,
  include_directories: server_public_headers,
  link_with: common_target,
  dependencies: enet
)
