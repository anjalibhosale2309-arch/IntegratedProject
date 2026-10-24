
#include <GL/glut.h>

void drawRectangle() {
    glBegin(GL_QUADS);
    glVertex2f(-50, -30);
    glVertex2f(50, -30);
    glVertex2f(50, 30);
    glVertex2f(-50, 30);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Original rectangle
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-180, 100, 0);
    drawRectangle();
    glPopMatrix();

    // Translation
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(0, 100, 0);
    drawRectangle();
    glPopMatrix();

    // Rotation
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(180, 100, 0);
    glRotatef(45, 0, 0, 1);
    drawRectangle();
    glPopMatrix();

    // Scaling
    glPushMatrix();
    glColor3f(0.0f, 0.5f, 1.0f);
    glTranslatef(0, -100, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    drawRectangle();
    glPopMatrix();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 500);
    glutCreateWindow("CG CO2 - Geometric Transformations");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-350, 350, -250, 250);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
