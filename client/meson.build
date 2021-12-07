client_sources = [
  'application.c',
  'client.c',
  'implementation.c',
  'initialise.c',
  'main.c',
  'player_mesh.c',
  'renderer.c'
]

client_public_headers = include_directories(
  '..',
  '../external'
)

client_dependencies = [
]

if build_machine.system() == 'linux'
  client_dependencies += dependency('threads')
  client_dependencies += dependency('gl')
  client_dependencies += dependency('glu')
  client_dependencies += dependency('x11')
  client_dependencies += dependency('xrandr')
  client_dependencies += dependency('xi')
endif

client_dependencies += dependency('glfw3')
client_dependencies += dependency('libenet')

build_args = [
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_IMPORTS'
endif

if build_machine.system() == 'linux'
  build_args += '-D_DEFAULT_SOURCE'
  build_args += '-Wno-missing-field-initializers'
endif

client_target = executable(
  'client',
  client_sources,
  include_directories: client_public_headers,
  c_args: build_args,
  link_with: common_target,
  dependencies: client_dependencies
)
