#ifndef CORE_UI_H
#define CORE_UI_H

#include <stdbool.h>

typedef struct {
    float x;
    float y;
    float width;
    float height;
    float color[4]; // RGBA
} UI_Box;

typedef struct UI_Element {
    UI_Box box;
    bool visible;
    struct UI_Element* next; // Linked list for multiple elements
} UI_Element;

typedef struct {
    UI_Element* root_elements;
} UI_Context;

void UI_InitContext(UI_Context* ctx);

UI_Element* UI_CreateBox(float x, float y, float w, float h, float r, float g, float b, float a);

void UI_AddElement(UI_Context* ctx, UI_Element* element);

void UI_ToggleVisibility(UI_Element* element);
void UI_SetVisible(UI_Element* element, bool visible);

void UI_Render(UI_Context* ctx);

#endif // CORE_UI_H

#ifdef UI_H_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <stdlib.h>

void UI_InitContext(UI_Context* ctx) {
    if (ctx) {
        ctx->root_elements = NULL;
    }
}

UI_Element* UI_CreateBox(float x, float y, float w, float h, float r, float g, float b, float a) {
    UI_Element* element = (UI_Element*)malloc(sizeof(UI_Element));
    if (!element) return NULL;

    element->box.x = x;
    element->box.y = y;
    element->box.width = w;
    element->box.height = h;
    element->box.color[0] = r;
    element->box.color[1] = g;
    element->box.color[2] = b;
    element->box.color[3] = a;

    element->visible = true;
    element->next = NULL;

    return element;
}

void UI_AddElement(UI_Context* ctx, UI_Element* element) {
    if (!ctx || !element) return;

    element->next = ctx->root_elements;
    ctx->root_elements = element;
}

void UI_ToggleVisibility(UI_Element* element) {
    if (element) {
        element->visible = !element->visible;
    }
}

void UI_SetVisible(UI_Element* element, bool visible) {
    if (element) {
        element->visible = visible;
    }
}

void UI_Render(UI_Context* ctx) {
    if (!ctx) return;

    UI_Element* curr = ctx->root_elements;

    // Simple rendering using legacy OpenGL for demonstration purposes
    while (curr != NULL) {
        if (curr->visible) {
            glColor4f(curr->box.color[0], curr->box.color[1], curr->box.color[2], curr->box.color[3]);

            glBegin(GL_QUADS);
            glVertex2f(curr->box.x, curr->box.y);
            glVertex2f(curr->box.x + curr->box.width, curr->box.y);
            glVertex2f(curr->box.x + curr->box.width, curr->box.y + curr->box.height);
            glVertex2f(curr->box.x, curr->box.y + curr->box.height);
            glEnd();
        }
        curr = curr->next;
    }
}
#endif
