#include "include/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct { float m[16]; } mat4;

mat4 mat4_identity() {
    mat4 res = {0};
    res.m[0] = 1.0f; res.m[5] = 1.0f; res.m[10] = 1.0f; res.m[15] = 1.0f;
    return res;
}

mat4 mat4_ortho(float left, float right, float bottom, float top) {
    mat4 res = mat4_identity();
    res.m[0] = 2.0f / (right - left);
    res.m[5] = 2.0f / (top - bottom);
    res.m[10] = -1.0f;
    res.m[12] = -(right + left) / (right - left);
    res.m[13] = -(top + bottom) / (top - bottom);
    return res;
}

void mat4_mul(float* out, float* a, float* b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out[i * 4 + j] = 0;
            for (int k = 0; k < 4; k++)
                out[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
        }
    }
}

const char* vShader = "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "uniform mat4 mvp;\n"
    "void main() { gl_Position = mvp * vec4(aPos, 0.0, 1.0); }";

const char* fShader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 color;\n"
    "void main() { FragColor = vec4(color, 1.0); }";

// --- State ---
bool mousePressed = false;

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) mousePressed = (action == GLFW_PRESS);
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "C OpenGL Button", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetMouseButtonCallback(window, mouse_callback);

    // Compile Shaders
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vShader, NULL); glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fShader, NULL); glCompileShader(fs);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs); glAttachShader(program, fs); glLinkProgram(program);

    // Geometry: Unit square (0,0 to 1,1)
    float vertices[] = { 0,1, 1,1, 1,0, 0,1, 1,0, 0,0 };
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Button Definition
    float btnX = 300, btnY = 250, btnW = 200, btnH = 100;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        my = 600 - my; // Screen flip

        bool hovered = (mx >= btnX && mx <= btnX + btnW && my >= btnY && my <= btnY + btnH);

        glUseProgram(program);

        // Calculate MVP Matrix: Projection * Model
        mat4 proj = mat4_ortho(0, 800, 0, 600);
        mat4 model = mat4_identity();
        // Manual Translation & Scaling in the model matrix
        model.m[12] = btnX; model.m[13] = btnY; // Translation
        model.m[0] = btnW;  model.m[5] = btnH;  // Scaling

        float mvp[16];
        mat4_mul(mvp, proj.m, model.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, mvp);

        // Color Logic
        if (hovered) {
            if (mousePressed) glUniform3f(glGetUniformLocation(program, "color"), 0.2f, 0.8f, 0.2f);
            else glUniform3f(glGetUniformLocation(program, "color"), 0.6f, 0.6f, 0.6f);
        } else {
            glUniform3f(glGetUniformLocation(program, "color"), 0.4f, 0.4f, 0.4f);
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
