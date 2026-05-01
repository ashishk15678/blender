project('blender', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

# Added core directories to the include path so the compiler finds your headers
incdir = include_directories('include', 'core', 'core/ui', 'core/video')

sources = [
  'src/main.c',
  'include/glad/src/glad.c',
  'core/ui/ui.c',
  'core/video/v4l2_editor.c' # Added the missing V4L2 implementation
]

executable('blender',
  sources,
  include_directories : incdir,
  dependencies : [glfw_dep, gl_dep, m_dep],
  # If you are using Linux-specific V4L2 features,
  # you usually don't need an extra dependency,
  # but some systems require threads:
  dependencies : [glfw_dep, gl_dep, m_dep, dependency('threads', required : false)],
)
