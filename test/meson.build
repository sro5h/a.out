test_dummy_sources = [
  'test_dummy.c'
]

test_dummy_public_headers = include_directories(
  '..',
  '../external'
)

build_args = [
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_IMPORTS'
endif

test('Dummy test', executable(
  'test_dummy',
  test_dummy_sources,
  include_directories: test_dummy_public_headers,
  link_with: common_target
))
