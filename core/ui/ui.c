#include "ui.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "../../include/stb/stb_easy_font.h"



void UI_InitContext(UI_Context* ctx) {
    if (ctx) {
        ctx->root_elements = NULL;
    }
}

UI_Element* UI_CreateBox(int id, float x, float y, float w, float h, float r, float g, float b, float a) {
    UI_Element* element = (UI_Element*)malloc(sizeof(UI_Element));
    if (!element) return NULL;

    element->id = id;
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

void UI_SetVisible(UI_Element* element, bool visible) {
    if (element) {
        element->visible = visible;
    }
}

bool UI_IsPointInside(UI_Element* element, float px, float py) {
    if (!element || !element->visible) return false;

    float bx = element->box.x;
    float by = element->box.y;
    float bw = element->box.width;
    float bh = element->box.height;

    return (px >= bx && px <= bx + bw && py >= by && py <= by + bh);
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

void UI_DrawString(float x, float y, const char* text, float scale, float r, float g, float b) {
    char buffer[10000]; // Buffer for ~150 chars
    int num_quads = stb_easy_font_print(0, 0, (char*)text, NULL, buffer, sizeof(buffer));

    glPushMatrix();

    glTranslatef(x, y, 0.0f);
    glScalef(scale, -scale, 1.0f); // Invert Y scale because stb_easy_font draws Y downwards, but OpenGL NDC is Y upwards

    glColor3f(r, g, b);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}
