test_buffer_sources = [
  'test_buffer.c'
]

test_byte_order_sources = [
  'test_byte_order.c'
]

test_dummy_sources = [
  'test_dummy.c'
]

test_public_headers = include_directories(
  '..',
  '../external'
)

build_args = [
]

if get_option('default_library') == 'shared'
  build_args += '-DCOMMON_IMPORTS'
endif

test('Buffer test', executable(
  'test_buffer',
  test_buffer_sources,
  include_directories: test_public_headers,
  link_with: common_target
))

test('Byte order test', executable(
  'test_byte_order',
  test_byte_order_sources,
  include_directories: test_public_headers,
  link_with: common_target
))

test('Dummy test', executable(
  'test_dummy',
  test_dummy_sources,
  include_directories: test_public_headers,
  link_with: common_target
))
