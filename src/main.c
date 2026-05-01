#include "core/screen/main.c"
#include "../core/ui/ui.h"
#include "../core/video/v4l2_editor.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
  STATE_MENU,
  STATE_2D_PROJECT,
  STATE_3D_PROJECT,
  STATE_VIDEO_EDITOR
} AppState;

void DrawRectOutline(float x, float y, float w, float h, float r, float g,
                     float b) {
  glColor3f(r, g, b);
  glLineWidth(2.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(x, y);
  glVertex2f(x + w, y);
  glVertex2f(x + w, y + h);
  glVertex2f(x, y + h);
  glEnd();
}

int main() {
  GLFWwindow *window = NULL;

  INIT_OPENGL();
  CREATE_SCREEN(window, 1280, 720, "C OpenGL Project Chooser");

  UI_Context ui_ctx;
  UI_InitContext(&ui_ctx);

  // --- UI INITIALIZATION ---
  UI_Element *menu_bg =
      UI_CreateBox(0, -1.0f, -1.0f, 2.0f, 2.0f, 0.1f, 0.1f, 0.15f, 1.0f);
  UI_AddElement(&ui_ctx, menu_bg);
  UI_Element *btn_2d =
      UI_CreateBox(1, -0.6f, -0.2f, 0.4f, 0.4f, 0.2f, 0.6f, 0.8f, 1.0f);
  UI_AddElement(&ui_ctx, btn_2d);
  UI_Element *btn_3d =
      UI_CreateBox(2, 0.2f, -0.2f, 0.4f, 0.4f, 0.8f, 0.3f, 0.2f, 1.0f);
  UI_AddElement(&ui_ctx, btn_3d);
  UI_Element *btn_vid_edit =
      UI_CreateBox(3, -0.2f, -0.7f, 0.4f, 0.2f, 0.8f, 0.4f, 0.2f, 1.0f);
  UI_AddElement(&ui_ctx, btn_vid_edit);
  UI_Element *btn_crop_apply =
      UI_CreateBox(4, -0.9f, 0.7f, 0.3f, 0.15f, 0.2f, 0.8f, 0.2f, 1.0f);
  UI_AddElement(&ui_ctx, btn_crop_apply);
  UI_Element *btn_crop_export =
      UI_CreateBox(6, -0.5f, 0.7f, 0.3f, 0.15f, 0.2f, 0.2f, 0.8f, 1.0f);
  UI_AddElement(&ui_ctx, btn_crop_export);
  UI_Element *btn_crop_reset =
      UI_CreateBox(5, -0.1f, 0.7f, 0.3f, 0.15f, 0.8f, 0.2f, 0.2f, 1.0f);
  UI_AddElement(&ui_ctx, btn_crop_reset);

  AppState current_state = STATE_MENU;
  bool left_mouse_was_pressed = false;

  float vid_x = -0.5f, vid_y = -0.5f, vid_w = 1.0f, vid_h = 1.0f;
  float orig_vid_x = vid_x, orig_vid_y = vid_y, orig_vid_w = vid_w,
        orig_vid_h = vid_h;
  bool is_cropping = false;
  float crop_sx = 0, crop_sy = 0, crop_cx = 0, crop_cy = 0;

  // --- CONTINUOUS LOOP ---
  BEGIN_FRAME(window)
  glClear(GL_COLOR_BUFFER_BIT);

  // Mouse Setup
  bool left_mouse_is_pressed =
      glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
  double mx, my;
  glfwGetCursorPos(window, &mx, &my);
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  float nx = (float)mx / width * 2.0f - 1.0f;
  float ny = -((float)my / height * 2.0f - 1.0f);

  switch (current_state) {
  case STATE_MENU:
    UI_SetVisible(menu_bg, true);
    UI_SetVisible(btn_2d, true);
    UI_SetVisible(btn_3d, true);
    UI_SetVisible(btn_vid_edit, false);
    UI_SetVisible(btn_crop_apply, false);
    UI_SetVisible(btn_crop_export, false);
    UI_SetVisible(btn_crop_reset, false);

    if (left_mouse_is_pressed && !left_mouse_was_pressed) {
      if (UI_IsPointInside(btn_2d, nx, ny))
        current_state = STATE_2D_PROJECT;
      else if (UI_IsPointInside(btn_3d, nx, ny))
        current_state = STATE_3D_PROJECT;
    }
    UI_Render(&ui_ctx);
    break;

  case STATE_2D_PROJECT:
    glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
    UI_SetVisible(menu_bg, false);
    UI_SetVisible(btn_2d, false);
    UI_SetVisible(btn_3d, false);
    UI_SetVisible(btn_vid_edit, true);

    if (left_mouse_is_pressed && !left_mouse_was_pressed) {
      if (UI_IsPointInside(btn_vid_edit, nx, ny)) {
        current_state = STATE_VIDEO_EDITOR;
        V4L2_InitVideoDecoder("/dev/video0");
        V4L2_LoadVideo("input_video.mp4");
      }
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      current_state = STATE_MENU;
    UI_Render(&ui_ctx);
    break;

  case STATE_VIDEO_EDITOR:
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    UI_SetVisible(btn_vid_edit, false);
    UI_SetVisible(btn_crop_apply, true);
    UI_SetVisible(btn_crop_export, true);
    UI_SetVisible(btn_crop_reset, true);

    // Drawing Video Box
    glColor3f(0.5f, 0.5f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(vid_x, vid_y);
    glVertex2f(vid_x + vid_w, vid_y);
    glVertex2f(vid_x + vid_w, vid_y + vid_h);
    glVertex2f(vid_x, vid_y + vid_h);
    glEnd();

    if (left_mouse_is_pressed) {
      if (!left_mouse_was_pressed) {
        if (UI_IsPointInside(btn_crop_apply, nx, ny)) {
          V4L2_ApplyCrop((int)((vid_x + 1.0f) * 960),
                         (int)((-vid_y + 1.0f) * 540), (int)(vid_w * 960),
                         (int)(vid_h * 540));
          is_cropping = false;
        } else if (UI_IsPointInside(btn_crop_export, nx, ny)) {
          V4L2_ExportVideo("exported_video.mp4");
        } else if (UI_IsPointInside(btn_crop_reset, nx, ny)) {
          vid_x = orig_vid_x;
          vid_y = orig_vid_y;
          vid_w = orig_vid_w;
          vid_h = orig_vid_h;
          is_cropping = false;
        } else {
          is_cropping = true;
          crop_sx = nx;
          crop_sy = ny;
        }
      }
      if (is_cropping) {
        crop_cx = nx;
        crop_cy = ny;
      }
    }

    if (is_cropping) {
      DrawRectOutline(crop_sx, crop_sy, crop_cx - crop_sx, crop_cy - crop_sy,
                      1.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      V4L2_Cleanup();
      current_state = STATE_2D_PROJECT;
    }
    UI_Render(&ui_ctx);
    break;

  case STATE_3D_PROJECT:
    glClearColor(0.6f, 0.2f, 0.2f, 1.0f);
    UI_SetVisible(btn_2d, false);
    UI_SetVisible(btn_3d, false);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      current_state = STATE_MENU;
    UI_Render(&ui_ctx);
    break;
  }

  left_mouse_was_pressed = left_mouse_is_pressed;
  END_FRAME(window)

  return 0;
}
