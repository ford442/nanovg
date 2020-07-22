import ctypes

GLEW_OK = 0
GLEW_NO_ERROR = 0
GLEW_ERROR_NO_GL_VERSION = 1 # missing GL version
GLEW_ERROR_GL_VERSION_10_ONLY = 2 # Need at least OpenGL 1.1
GLEW_ERROR_GLX_VERSION_11_ONLY = 3 # Need at least GLX 1.2
GLEW_ERROR_NO_GLX_DISPLAY = 4 # Need GLX display for GLX support

GLEW_VERSION = 1
GLEW_VERSION_MAJOR = 2
GLEW_VERSION_MINOR = 3
GLEW_VERSION_MICRO = 4
