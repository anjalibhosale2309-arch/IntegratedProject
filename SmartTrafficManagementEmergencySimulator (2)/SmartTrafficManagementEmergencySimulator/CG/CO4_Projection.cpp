
#include <GL/glut.h>

void drawCube(float size, float x, float y, float z) {
    float s = size / 2.0f;

    glBegin(GL_QUADS);

    // Front
    glVertex3f(x-s, y-s, z+s);
    glVertex3f(x+s, y-s, z+s);
    glVertex3f(x+s, y+s, z+s);
    glVertex3f(x-s, y+s, z+s);

    // Back
    glVertex3f(x-s, y-s, z-s);
    glVertex3f(x-s, y+s, z-s);
    glVertex3f(x+s, y+s, z-s);
    glVertex3f(x+s, y-s, z-s);

    // Top
    glVertex3f(x-s, y+s, z-s);
    glVertex3f(x-s, y+s, z+s);
    glVertex3f(x+s, y+s, z+s);
    glVertex3f(x+s, y+s, z-s);

    // Bottom
    glVertex3f(x-s, y-s, z-s);
    glVertex3f(x+s, y-s, z-s);
    glVertex3f(x+s, y-s, z+s);
    glVertex3f(x-s, y-s, z+s);

    // Right
    glVertex3f(x+s, y-s, z-s);
    glVertex3f(x+s, y+s, z-s);
    glVertex3f(x+s, y+s, z+s);
    glVertex3f(x+s, y-s, z+s);

    // Left
    glVertex3f(x-s, y-s, z-s);
    glVertex3f(x-s, y-s, z+s);
    glVertex3f(x-s, y+s, z+s);
    glVertex3f(x-s, y+s, z-s);

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Left: Orthographic projection
    glViewport(0, 0, 350, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3, 3, -3, 3, 1, 20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4, 3, 7, 0, 0, 0, 0, 1, 0);

    glColor3f(0.2f, 0.8f, 1.0f);
    drawCube(2.0f, 0, 0, 0);

    // Right: Perspective projection
    glViewport(350, 0, 350, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 350.0 / 500.0, 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4, 3, 7, 0, 0, 0, 0, 1, 0);

    glColor3f(1.0f, 0.6f, 0.2f);
    drawCube(2.0f, 0, 0, 0);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 500);
    glutCreateWindow("CG CO4 - Projection and Viewing");

    glClearColor(0.08f, 0.08f, 0.08f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

