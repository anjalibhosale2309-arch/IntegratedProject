
#include <GL/glut.h>
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Red line
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(50, 350);
    glVertex2f(250, 350);
    glEnd();

    // Green triangle
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(300, 300);
    glVertex2f(400, 400);
    glVertex2f(500, 300);
    glEnd();

    // Blue rectangle
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(100, 100);
    glVertex2f(250, 100);
    glVertex2f(250, 200);
    glVertex2f(100, 200);
    glEnd();

    // Yellow circle
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(400, 150);

    for (int i = 0; i <= 100; i++) {
        float angle = i * 6.2831853f / 100;
        glVertex2f(
            400 + 50 * cosf(angle),
            150 + 50 * sinf(angle)
        );
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 450);
    glutCreateWindow("CG CO1 - OpenGL Primitives");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 450);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
