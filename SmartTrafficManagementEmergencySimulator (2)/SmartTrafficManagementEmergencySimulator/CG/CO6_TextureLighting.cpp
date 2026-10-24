
#include <GL/glut.h>
#include <iostream>
using namespace std;

const int TEXTURE_SIZE = 64;

unsigned char image[TEXTURE_SIZE][TEXTURE_SIZE][3];
GLuint textureID;

// Create checkerboard texture
void createTexture()
{
    for (int i = 0; i < TEXTURE_SIZE; i++)
    {
        for (int j = 0; j < TEXTURE_SIZE; j++)
        {
            int color = ((i / 8) + (j / 8)) % 2;

            for (int k = 0; k < 3; k++)
            {
                image[i][j][k] = color ? 255 : 40;
            }
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 TEXTURE_SIZE, TEXTURE_SIZE, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image);
}

// Draw cube with texture
void drawCube()
{
    glBegin(GL_QUADS);

    // Front face
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);

    // Back face
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, -1);

    // Top face
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, 1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, 1, 1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, 1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, -1);

    // Bottom face
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(1, -1, -1);
    glTexCoord2f(0, 1); glVertex3f(1, -1, 1);

    // Right face
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
    glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
    glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
    glTexCoord2f(0, 1); glVertex3f(1, 1, 1);

    // Left face
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
    glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
    glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
    glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);

    glEnd();
}

// Display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(3, 3, 6,
              0, 0, 0,
              0, 1, 0);

    glRotatef(25, 1, 1, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glColor3f(1, 1, 1);
    drawCube();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

// Initialize OpenGL
void init()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT_AND_BACK,
                    GL_AMBIENT_AND_DIFFUSE);

    GLfloat lightPosition[] = {2, 4, 5, 1};
    GLfloat whiteLight[] = {1, 1, 1, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, 700.0 / 500.0, 1, 100);

    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    createTexture();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH
    );

    glutInitWindowSize(700, 500);

    glutCreateWindow(
        "CG CO6 - Texture Mapping and Lighting"
    );

    init();

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
