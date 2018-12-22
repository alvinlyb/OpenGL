#ifndef IMPORT_OPENGL3_HXX
#define IMPORT_OPENGL3_HXX

//set use opengl
#if !defined(IMPORT_OPENGL_LOADER_GL3W) \
 && !defined(IMPORT_OPENGL_LOADER_GLEW) \
 && !defined(IMPORT_OPENGL_LOADER_GLAD) \
 && !defined(IMPORT_OPENGL_LOADER_GLFW) \
 && !defined(IMPORT_OPENGL_LOADER_CUSTOM)
#define IMPORT_OPENGL_LOADER_CUSTOM
#endif

//#define IMPORT_LOADER_SDL

//include opengl haderFile
#if defined(IMPORT_OPENGL_LOADER_GL3W)
#include <GLFW/glf3w.h>
#elif defined(IMPORT_OPENGL_LOADER_GLEW)
#include <gl/glew.h>
#elif defined(IMPORT_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#else
#include IMPORT_OPENGL_LOADER_CUSTOM
#endif

#if defined(IMPORT_LOADER_SDL)
#include <SDL.h>
#include <SDL_opengl.h>
#elif defined(IMPORT_LOADER_GLFW)
#include <GLFW/glfw3.h>
#endif

//Initialize opengl loader
#if defined(IMPORT_OPENGL_LOADER_GL3W)
    bool err=gl3wInit()!=0;
#elif defined(IMPORT_OPENGL_LOADER_GLEW)
    bool err=glewInit()!=GLEW_OK;
#elif defined(IMPORT_OPENGL_LOADER_GLFW)
    bool err=glfwInit()!=0;
#elif defined(IMPORT_OPENGL_LOADER_GLAD)
    bool err=gladLoadGL()==0;
#endif

//opengl math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//load stb image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#endif // IMPORT_OPENGL3_HXX
