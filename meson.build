project('blender', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')
incdir = include_directories('include')

sources = [
  'src/main.c',
  'include/glad/src/glad.c'
]

executable('blender',
  sources,
  include_directories : incdir,
  dependencies : [glfw_dep, gl_dep],
)
