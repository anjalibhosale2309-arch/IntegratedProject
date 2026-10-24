
#include <GL/glut.h>
#include <iostream>
#include <algorithm>
using namespace std;

const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

const float xmin = 150, ymin = 100;
const float xmax = 450, ymax = 350;

int computeCode(float x, float y) {
    int code = 0;

    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;

    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;

    return code;
}

void cohenSutherland(float x1, float y1,
                     float x2, float y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true) {
        if (code1 == 0 && code2 == 0) {
            accept = true;
            break;
        }
        else if (code1 & code2) {
            break;
        }
        else {
            int codeOut = code1 ? code1 : code2;
            float x, y;

            if (codeOut & TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    if (accept) {
        glColor3f(0.0f, 0.8f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Clipping window
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();

    // Original lines in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(50, 50);
    glVertex2f(500, 400);

    glVertex2f(200, 150);
    glVertex2f(400, 300);

    glVertex2f(50, 300);
    glVertex2f(100, 400);
    glEnd();

    // Clipped accepted lines in green
    cohenSutherland(50, 50, 500, 400);
    cohenSutherland(200, 150, 400, 300);
    cohenSutherland(50, 300, 100, 400);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 450);
    glutCreateWindow("CO5 - Cohen Sutherland Line Clipping");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 450);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
