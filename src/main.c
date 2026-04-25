#include "core/screen/main.c"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#define UI_H_IMPLEMENTATION
#include "../core/ui/ui.h"

int main() {
  GLFWwindow *window = NULL;

  INIT_OPENGL();
  CREATE_SCREEN(window, 1280, 720, "C OpenGL Window");

  UI_Context ui_ctx;
  UI_InitContext(&ui_ctx);

  UI_Element* menu_bg = UI_CreateBox(-0.5f, -0.5f, 1.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.8f);
  UI_AddElement(&ui_ctx, menu_bg);

  UI_Element* popup_box = UI_CreateBox(-0.2f, -0.2f, 0.4f, 0.4f, 0.8f, 0.3f, 0.3f, 1.0f);
  UI_SetVisible(popup_box, false);
  UI_AddElement(&ui_ctx, popup_box);

  BEGIN_FRAME(window)

  glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      UI_SetVisible(popup_box, true);
  } else {
      UI_SetVisible(popup_box, false);
  }

  UI_Render(&ui_ctx);

  END_FRAME(window)

  return 0;
}
