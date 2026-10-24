
#include "CG.h"
#include <GL/glut.h>
#include <cmath>
#include <sstream>
#include <string>
#include <cstdlib>

static TrafficController* traffic = nullptr;
static float ambulanceProgress = 0.0f;
static int previousEmergency = -1;
static float signalScale = 1.0f;
static float scaleStep = 0.01f;

static void drawText(float x, float y, const std::string& s) {
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);

    for (unsigned char c : s) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

static void rect(float x1, float y1, float x2, float y2,
                 float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

static void circle(float x, float y, float rad,
                   float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);

    for (int i = 0; i <= 32; ++i) {
        float a = i * 6.2831853f / 32.0f;
        glVertex2f(x + rad * cosf(a),
                   y + rad * sinf(a));
    }

    glEnd();
}

static void signal(float x, float y, int lane, int green) {
    glPushMatrix();

    // Scale relative to the signal's center
    

    rect(x - 16, y - 28, x + 16, y + 28,
         0.08f, 0.08f, 0.08f);

    circle(x, y + 12, 7,
           lane == green ? 0.05f : 0.85f,
           lane == green ? 0.95f : 0.05f,
           0.05f);

    circle(x, y - 9, 7,
           lane == green ? 0.15f : 0.95f,
           0.05f, 0.05f);

    glPopMatrix();
}

static void car(float x, float y, bool horizontal,
                bool ambulance = false) {
    float length = ambulance ? 18.0f : 13.0f;
    float width = ambulance ? 8.0f : 6.0f;

    if (!horizontal) {
        float t = length;
        length = width;
        width = t;
    }

    if (ambulance) {
        rect(x - length, y - width, x + length, y + width,
             0.95f, 0.95f, 0.95f);

        rect(x - 6, y - 3, x + 6, y + 3,
             0.85f, 0.05f, 0.05f);

        rect(x - 3, y - width - 2, x + 3, y - width + 2,
             0.1f, 0.3f, 1.0f);
    } else {
        rect(x - length, y - width, x + length, y + width,
             0.12f, 0.55f, 1.0f);
    }
}
static void ambulance(float x, float y, bool horizontal) {
    glPushMatrix();

    glTranslatef(x, y, 0.0f);
    glScalef(signalScale, signalScale, 1.0f);
    glTranslatef(-x, -y, 0.0f);

    car(x, y, horizontal, true);

    glPopMatrix();
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Background
    rect(0, 0, 800, 600, 0.12f, 0.38f, 0.16f);

    // Roads
    rect(0, 230, 800, 370, 0.20f, 0.20f, 0.22f);
    rect(330, 0, 470, 600, 0.20f, 0.20f, 0.22f);

    // Road markings
    for (int x = 10; x < 320; x += 45)
        rect(x, 298, x + 24, 302, 0.95f, 0.90f, 0.55f);

    for (int x = 480; x < 800; x += 45)
        rect(x, 298, x + 24, 302, 0.95f, 0.90f, 0.55f);

    for (int y = 10; y < 220; y += 45)
        rect(398, y, 402, y + 24, 0.95f, 0.90f, 0.55f);

    for (int y = 380; y < 600; y += 45)
        rect(398, y, 402, y + 24, 0.95f, 0.90f, 0.55f);

    int green = traffic->getGreenLane();
    int emergency = traffic->getEmergencyLane();

    // Title and directions
    drawText(240, 575, "SMART TRAFFIC MANAGEMENT");
    drawText(270, 550, "AND EMERGENCY SIMULATOR");

    drawText(345, 410, "NORTH");
    drawText(345, 190, "SOUTH");
    drawText(65, 390, "WEST");
    drawText(660, 390, "EAST");

    // Traffic signals
    signal(485, 410, EAST, green);
    signal(315, 410, WEST, green);
    signal(485, 190, SOUTH, green);
    signal(315, 190, NORTH, green);

    // Vehicles
    car(60 + traffic->getVehiclePosition(EAST) * 240,
        270, true);

    car(740 - traffic->getVehiclePosition(WEST) * 240,
        330, true);

    car(370, 30 + traffic->getVehiclePosition(SOUTH) * 180,
        false);

    car(430, 570 - traffic->getVehiclePosition(NORTH) * 180,
        false);

    // Emergency ambulance with movement and scaling
if (emergency == EAST) {
    ambulance(250 + ambulanceProgress, 270, true);
}
else if (emergency == WEST) {
    ambulance(550 - ambulanceProgress, 330, true);
}
else if (emergency == SOUTH) {
    ambulance(370, 150 + ambulanceProgress, false);
}
else if (emergency == NORTH) {
    ambulance(430, 450 - ambulanceProgress, false);
}

    // Queue information
    const char* names[4] = {
        "EAST", "WEST", "SOUTH", "NORTH"
    };

    for (int i = 0; i < 4; ++i) {
        std::ostringstream out;
        out << names[i] << " queue: " << traffic->getQueueSize(i);
        drawText(15, 165 - i * 24, out.str());
    }

    // Emergency status
    if (emergency != -1) {
        drawText(520, 160, "EMERGENCY OVERRIDE!");

        drawText(520, 135,
                 std::string("GREEN: ") + names[emergency]);
    } else {
        drawText(520, 160, "NORMAL SIGNAL MODE");
    }

    drawText(15, 35, "E: next emergency");
    drawText(15, 15,
             "1 East | 2 West | 3 South | 4 North | Esc Exit");

    glutSwapBuffers();
}
static void keyboard(unsigned char key, int, int) {
    if (key == 'e' || key == 'E') {
        traffic->triggerNextEmergency();
    }
    else if (key >= '1' && key <= '4') {
        traffic->triggerEmergency(key - '1');
    }
    else if (key == 27) {
        std::exit(0);
    }

    glutPostRedisplay();
}

static void timer(int) {
    int emergency = traffic->getEmergencyLane();

    if (emergency == -1) {
        ambulanceProgress = 0.0f;
    }
    else if (emergency != previousEmergency) {
        ambulanceProgress = 0.0f;
    }
    else {
        ambulanceProgress += 3.0f;

        if (ambulanceProgress > 150.0f) {
            ambulanceProgress = 150.0f;
        }
    }

    previousEmergency = emergency;

    traffic->update();
    signalScale += scaleStep;

if (signalScale >= 1.5f) {
    signalScale = 1.5f;
    scaleStep = -0.01f;
}

if (signalScale <= 1.0f) {
    signalScale = 1.0f;
    scaleStep = 0.01f;
}

    glutPostRedisplay();
    glutTimerFunc(50, timer, 0);
}

void runGraphics(TrafficController& controller) {
    traffic = &controller;

    int argc = 1;
    char appName[] = "Traffic Simulator";
    char* argv[] = {appName, nullptr};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 50);

    glutCreateWindow(
        "Smart Traffic Management and Emergency Simulator"
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(50, timer, 0);

    glutMainLoop();
}
