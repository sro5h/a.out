test_byte_order_sources = [
  'test_byte_order.c'
]

test_dummy_sources = [
  'test_dummy.c'
]

test_stream_sources = [
  'test_stream.c'
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

test('Stream test', executable(
  'test_stream',
  test_stream_sources,
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
