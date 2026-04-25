#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_OPENGL() \
    if (!glfwInit()) { \
        fprintf(stderr, "Error: Could not initialize GLFW\n"); \
        exit(EXIT_FAILURE); \
    }

#define CREATE_SCREEN(window_ptr, width, height, title) \
    window_ptr = glfwCreateWindow(width, height, title, NULL, NULL); \
    if (!window_ptr) { \
        fprintf(stderr, "Error: Could not create GLFW window\n"); \
        glfwTerminate(); \
        exit(EXIT_FAILURE); \
    } \
    glfwMakeContextCurrent(window_ptr);

#define BEGIN_FRAME(window_ptr) \
    while (!glfwWindowShouldClose(window_ptr)) { \
        glClear(GL_COLOR_BUFFER_BIT);

#define END_FRAME(window_ptr) \
        glfwSwapBuffers(window_ptr); \
        glfwPollEvents(); \
    } \
    glfwTerminate();

#endif
