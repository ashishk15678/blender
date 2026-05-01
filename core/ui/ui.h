#ifndef CORE_UI_H
#define CORE_UI_H

#include <stdbool.h>

// Represents a basic UI rectangle/box
typedef struct {
    float x;
    float y;
    float width;
    float height;
    float color[4]; // RGBA
} UI_Box;

// Represents a general UI element that can be toggled
typedef struct UI_Element {
    int id; // Optional ID for interactions
    UI_Box box;
    bool visible;
    struct UI_Element* next; // Linked list for multiple elements
} UI_Element;

// Represents the overall UI state
typedef struct {
    UI_Element* root_elements;
} UI_Context;

// Initialize the UI context
void UI_InitContext(UI_Context* ctx);



// Create a new box element
UI_Element* UI_CreateBox(int id, float x, float y, float w, float h, float r, float g, float b, float a);

// Add an element to the UI context to be rendered
void UI_AddElement(UI_Context* ctx, UI_Element* element);

// Toggle visibility of an element
void UI_SetVisible(UI_Element* element, bool visible);

// Check if a normalized mouse point is inside a box
bool UI_IsPointInside(UI_Element* element, float px, float py);

// Render all visible elements in the UI context
void UI_Render(UI_Context* ctx);

// Render an arbitrary text string
void UI_DrawString(float x, float y, const char* text, float scale, float r, float g, float b);

#endif // CORE_UI_H
