project('fake-ext4', 'c',
  default_options: [
    'werror=true',
    'optimization=g',
  ]
)

codegen_flags = [
  '-ggdb3', '-fvar-tracking-assignments',
]

common_flags = [
  '-Wall', '-Wextra',
  '-Wold-style-definition', '-Wundef', '-Wmissing-prototypes', '-Wstrict-prototypes',
]

add_project_arguments(
  common_flags,
  codegen_flags,
  language: 'c'
)

cc = meson.get_compiler('c')
dl = cc.find_library('dl', required: false)

libfake_ext4 = shared_library('fake_ext4',
  'libfake_ext4.c',
  dependencies: [dl]
)

fake_ext4 = executable('fake_ext4',
  'fake_ext4.c'
)
