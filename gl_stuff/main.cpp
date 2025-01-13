
#include <GLFW/glfw3.h>
#include <iostream>
#define _USE_MATH_DEFINES  // for some reason CONSTANTS won't pull in if this is not declared
#include <cmath>

using namespace std;

void drawLine(float x1, float y1, float x2, float y2) {
    glColor3f(0, 1, 0);

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawCircle(float centerX, float centerY, float radius) {
    glColor3f(1, 0, 0);

    glBegin(GL_POLYGON);
    for(int i=0; i<360; i++) {
        float angle = i * M_PI / 180.0f;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);

        glVertex2f(x, y);
    }
    glEnd();
}

void key_listener(GLFWwindow *win, int key, int scancode, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(win, &xpos, &ypos);
    cout << xpos << ", " << ypos << endl;

    if(key == GLFW_KEY_W && action == GLFW_PRESS) {}

    cout << "key index: " << key << " scan code: " << scancode << " action: " << action << " mods: " << mods << endl;
}

void mouse_listener(GLFWwindow *win, int btn, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(win, &xpos, &ypos);
    cout << xpos << ", " << ypos << endl;

    if(btn == GLFW_MOUSE_BUTTON_LEFT) {
        // pew pew
        cout << "pew pew" << endl;
    }
}

void do_gl() {
    // initialize GLFW
    if(!glfwInit()) {
        cerr << "failed to initialize GLFW" << endl;
        exit(-1);
    }

    // don't allow window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // create a window
    GLFWwindow *win = glfwCreateWindow(640, 480, "my window", NULL, NULL);
    if(!win) {
        cerr << "failed to create GLFW window" << endl;
        glfwTerminate();
        exit(-1);
    }

    // make window current context
    glfwMakeContextCurrent(win);

    // key listener
    glfwSetKeyCallback(win, key_listener);

    // mouse listener
    glfwSetMouseButtonCallback(win, mouse_listener);

    glViewport(0, 0, 640, 480);

    // drawing loop
    while(!glfwWindowShouldClose(win)) {
        // clear the screen
        glClear(GL_COLOR_BUFFER_BIT);
        // draw a line
        drawLine(-1.0f, -1.0f, 1.0f, 1.0f);
        // draw a circle
        drawCircle(0.5f, 0.5f, 0.2f);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
}

int main(int argc, char *argv[]) {
    do_gl();

    return 0;
}