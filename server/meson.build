server_sources = [
  'application.c',
  'main.c',
  'implementation.c',
  'initialise.c',
  'server.c'
]

server_public_headers = include_directories(
  '..',
  '../external'
)

server_dependencies = [
]

if build_machine.system() == 'linux'
  server_dependencies += meson.get_compiler('c').find_library('m')
  server_dependencies += dependency('threads')
endif

server_dependencies += dependency('libchipmunk')
server_dependencies += dependency('libenet')

build_args = [
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_IMPORTS'
endif

if build_machine.system() == 'linux'
  build_args += '-D_DEFAULT_SOURCE'
endif

server_target = executable(
  'server',
  server_sources,
  include_directories: server_public_headers,
  c_args: build_args,
  link_with: common_target,
  dependencies: server_dependencies
)
