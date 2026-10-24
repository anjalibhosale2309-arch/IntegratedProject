// ==============================================================================
// MIT ACADEMY OF ENGINEERING, ALANDI (PUNE)
// Department of Computer Engineering
// Second Year B.Tech - Computer Graphics (2310218T / 2310218L)
//
// Mapped Course Outcomes:
//   - CO1: OpenGL Primitives (GL_QUADS, GL_LINES, GL_TRIANGLE_FAN, Bitmap Text)
//   - CO2: Geometric Transformations (glTranslatef, glRotatef, glScalef)
//   - CO3: 2D Objects & Algorithms (Bresenham circle, intersection, signals)
//   - CO4: Orthographic Projection (gluOrtho2D, glViewport coordinate scaling)
//   - CO5: Clipping Algorithms (Cohen-Sutherland Line Clipping & Scissor Test)
//   - CO6: Texture Mapping, Lighting (Diffused signal glow, RGB color models)
// ==============================================================================

#include "CG.h"
#include <GL/glut.h>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global Simulator State
static TrafficController* g_traffic = nullptr;
static ViewTab g_currentTab = TAB_LANDING;

static int g_windowWidth = 1200;
static int g_windowHeight = 720;
static int g_mouseX = 0;
static int g_mouseY = 0;
static int g_tick = 0;
static float g_pulse = 1.0f;
static float g_pulseStep = 0.02f;

// DSA Laboratory State
static int g_sortArray[6] = {105, 101, 104, 102, 106, 103};
static int g_sortI = 0, g_sortJ = 0;
static bool g_sortDone = false;
static std::string g_hashStatusMsg = "Hash table ready: formula h(key) = key % 10";
static std::string g_sortStatusMsg = "Bubble Sort & Linear Search ready";
static int g_registrySortMode = 0; // 0=Priority, 1=Time, 2=Type, 3=ID

// Interactive Clickable Button Definition
struct InteractiveButton {
    int id;
    float x, y, w, h;
    std::string text;
};

static std::vector<InteractiveButton> g_activeButtons;

// ------------------------------------------------------------------------------
// CG CO1: Drawing Primitives (Bitmap Text, Quads, Outlines)
// ------------------------------------------------------------------------------
static void drawText(float x, float y, const std::string& str, void* font = GLUT_BITMAP_HELVETICA_12, float r = 1.0f, float g = 1.0f, float b = 1.0f) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, (unsigned char)c);
    }
}

static void drawRect(float x1, float y1, float x2, float y2, float r, float g, float b, float alpha = 1.0f) {
    glColor4f(r, g, b, alpha);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

static void drawRectOutline(float x1, float y1, float x2, float y2, float r, float g, float b, float width = 1.2f) {
    glLineWidth(width);
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
    glLineWidth(1.0f);
}

// ------------------------------------------------------------------------------
// CG CO3: Midpoint / Bresenham Circle Algorithm
// ------------------------------------------------------------------------------
static void drawBresenhamCircle(float cx, float cy, float radius, float r, float g, float b, bool filled = true) {
    glColor3f(r, g, b);
    if (filled) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 36; ++i) {
            float angle = i * 2.0f * (float)M_PI / 36.0f;
            glVertex2f(cx + radius * cosf(angle), cy + radius * sinf(angle));
        }
        glEnd();
    } else {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= 36; ++i) {
            float angle = i * 2.0f * (float)M_PI / 36.0f;
            glVertex2f(cx + radius * cosf(angle), cy + radius * sinf(angle));
        }
        glEnd();
    }
}

// ------------------------------------------------------------------------------
// CG CO6: Diffused Illumination & Lens Light Glow Simulation
// ------------------------------------------------------------------------------
static void drawLightGlow(float cx, float cy, float radius, float r, float g, float b, float intensity = 0.45f) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, intensity);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    glColor4f(r, g, b, 0.0f);
    for (int i = 0; i <= 24; ++i) {
        float angle = i * 2.0f * (float)M_PI / 24.0f;
        glVertex2f(cx + radius * cosf(angle), cy + radius * sinf(angle));
    }
    glEnd();
    glDisable(GL_BLEND);
}

// ------------------------------------------------------------------------------
// CG CO5: Cohen-Sutherland Line Clipping Algorithm
// ------------------------------------------------------------------------------
static const int CS_INSIDE = 0;
static const int CS_LEFT = 1;
static const int CS_RIGHT = 2;
static const int CS_BOTTOM = 4;
static const int CS_TOP = 8;

static int computeCSCode(float x, float y, float xmin, float ymin, float xmax, float ymax) {
    int code = CS_INSIDE;
    if (x < xmin) code |= CS_LEFT;
    else if (x > xmax) code |= CS_RIGHT;
    if (y < ymin) code |= CS_BOTTOM;
    else if (y > ymax) code |= CS_TOP;
    return code;
}

static void drawClippedLine(float x1, float y1, float x2, float y2, float xmin, float ymin, float xmax, float ymax, float r, float g, float b, float width = 1.5f) {
    int code1 = computeCSCode(x1, y1, xmin, ymin, xmax, ymax);
    int code2 = computeCSCode(x2, y2, xmin, ymin, xmax, ymax);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            accept = true;
            break;
        } else if (code1 & code2) {
            break;
        } else {
            int codeOut = code1 ? code1 : code2;
            float x = 0, y = 0;

            if (codeOut & CS_TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            } else if (codeOut & CS_BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (codeOut & CS_RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            } else if (codeOut & CS_LEFT) {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCSCode(x1, y1, xmin, ymin, xmax, ymax);
            } else {
                x2 = x;
                y2 = y;
                code2 = computeCSCode(x2, y2, xmin, ymin, xmax, ymax);
            }
        }
    }

    if (accept) {
        glLineWidth(width);
        glColor3f(r, g, b);
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
        glLineWidth(1.0f);
    }
}

// Standard Card Background Container
static void drawCard(float x1, float y1, float x2, float y2, float bgR = 0.08f, float bgG = 0.11f, float bgB = 0.18f, float borderR = 0.18f, float borderG = 0.22f, float borderB = 0.32f) {
    drawRect(x1, y1, x2, y2, bgR, bgG, bgB);
    drawRectOutline(x1, y1, x2, y2, borderR, borderG, borderB, 1.2f);
}

// ------------------------------------------------------------------------------
// Unified Interactive Button: Registers hitbox & Renders in 1 atomic call
// Prevents hit-test coordinate desync and double-drawing bugs!
// ------------------------------------------------------------------------------
static void drawInteractiveButton(int id, float x, float y, float w, float h, const std::string& text, float r, float g, float b, bool active = false, bool outlineOnly = false) {
    g_activeButtons.push_back({id, x, y, w, h, text});

    bool hover = (g_mouseX >= x && g_mouseX <= x + w && g_mouseY >= y && g_mouseY <= y + h);

    if (active) {
        drawRect(x, y, x + w, y + h, 0.15f, 0.42f, 0.92f);
        drawRectOutline(x, y, x + w, y + h, 0.40f, 0.70f, 1.0f, 1.5f);
        drawRect(x, y + 4, x + 3.5f, y + h - 4, 1.0f, 1.0f, 1.0f);
        float textX = x + (w - text.length() * 6.5f) * 0.5f;
        float textY = y + (h - 10.0f) * 0.5f;
        drawText(std::max(x + 6.0f, textX), textY, text, GLUT_BITMAP_HELVETICA_12, 1.0f, 1.0f, 1.0f);
    } else if (outlineOnly) {
        drawRect(x, y, x + w, y + h, hover ? 0.16f : 0.08f, hover ? 0.22f : 0.12f, hover ? 0.32f : 0.19f);
        drawRectOutline(x, y, x + w, y + h, hover ? std::min(1.0f, r * 1.3f) : r, hover ? std::min(1.0f, g * 1.3f) : g, hover ? std::min(1.0f, b * 1.3f) : b, hover ? 1.8f : 1.2f);
        float textX = x + (w - text.length() * 6.5f) * 0.5f;
        float textY = y + (h - 10.0f) * 0.5f;
        drawText(std::max(x + 6.0f, textX), textY, text, GLUT_BITMAP_HELVETICA_12, hover ? 1.0f : r, hover ? 1.0f : g, hover ? 1.0f : b);
    } else {
        float drawR = hover ? std::min(1.0f, r * 1.25f) : r;
        float drawG = hover ? std::min(1.0f, g * 1.25f) : g;
        float drawB = hover ? std::min(1.0f, b * 1.25f) : b;
        drawRect(x, y, x + w, y + h, drawR, drawG, drawB);
        drawRectOutline(x, y, x + w, y + h, hover ? 1.0f : drawR * 0.8f, hover ? 1.0f : drawG * 0.8f, hover ? 1.0f : drawB * 0.8f, hover ? 1.8f : 1.0f);
        float textX = x + (w - text.length() * 6.5f) * 0.5f;
        float textY = y + (h - 10.0f) * 0.5f;
        drawText(std::max(x + 6.0f, textX), textY, text, GLUT_BITMAP_HELVETICA_12, 1.0f, 1.0f, 1.0f);
    }
}

// ------------------------------------------------------------------------------
// Navigation Sidebar (x: 0 to 225, y: 0 to 720)
// Matches ui2code prototype: https://b-011391a3-4c79-4014-b146-e62a0dc842db-q8ek6qbkgvezm6mx.ui2code.ai/
// ------------------------------------------------------------------------------
static void renderSidebar() {
    float sbW = 225.0f;
    drawRect(0, 0, sbW, 720, 0.059f, 0.090f, 0.165f);
    drawClippedLine(sbW, 0, sbW, 720, 0, 0, 1200, 720, 0.20f, 0.25f, 0.35f, 1.5f);

    // Brand Logo & Title
    drawRect(18, 662, 52, 696, 0.15f, 0.45f, 0.95f);
    drawRectOutline(18, 662, 52, 696, 0.35f, 0.65f, 1.0f, 1.5f);
    drawBresenhamCircle(35, 685, 4.0f, 1.0f, 0.2f, 0.2f, true);
    drawBresenhamCircle(35, 673, 4.0f, 0.2f, 1.0f, 0.3f, true);

    drawText(62, 682, "SmartTraffic", GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);
    drawText(62, 666, "Simulator v1.0", GLUT_BITMAP_HELVETICA_10, 0.58f, 0.64f, 0.72f);
    drawClippedLine(15, 650, sbW - 15, 650, 0, 0, 1200, 720, 0.15f, 0.20f, 0.28f);

    // Navigation Tab Buttons
    struct TabItem {
        ViewTab tab;
        int id;
        std::string label;
    };
    TabItem tabs[4] = {
        {TAB_LANDING, 10, "Landing Hub"},
        {TAB_DASHBOARD, 11, "Live Dashboard"},
        {TAB_REGISTRY, 12, "Vehicle Registry"},
        {TAB_DSA_LAB, 13, "DSA Laboratory"}
    };

    float navY = 595.0f;
    for (int i = 0; i < 4; ++i) {
        bool isActive = (g_currentTab == tabs[i].tab);
        float bx = 14.0f, bw = sbW - 28.0f, bh = 38.0f;
        drawInteractiveButton(tabs[i].id, bx, navY, bw, bh, tabs[i].label, 0.18f, 0.24f, 0.35f, isActive, !isActive);
        navY -= 46.0f;
    }

    // System Status Card
    drawText(18, 400, "SYSTEM", GLUT_BITMAP_HELVETICA_10, 0.45f, 0.52f, 0.60f);
    drawCard(14, 275, sbW - 14, 385, 0.08f, 0.12f, 0.19f, 0.18f, 0.25f, 0.35f);

    drawText(24, 358, "Engine", GLUT_BITMAP_HELVETICA_12, 0.75f, 0.80f, 0.88f);
    float pulseAlpha = 0.65f + 0.35f * sinf(g_tick * 0.15f);
    drawBresenhamCircle(145, 362, 4.5f, 0.1f * pulseAlpha, 0.95f * pulseAlpha, 0.4f * pulseAlpha, true);
    drawText(155, 358, "Online", GLUT_BITMAP_HELVETICA_12, 0.2f, 0.95f, 0.45f);

    drawText(24, 332, "Vehicles", GLUT_BITMAP_HELVETICA_12, 0.75f, 0.80f, 0.88f);
    int activeVehiclesCount = (int)g_traffic->getVehicles().size();
    drawText(160, 332, std::to_string(activeVehiclesCount), GLUT_BITMAP_HELVETICA_12, 0.15f, 0.85f, 0.95f);

    drawText(24, 306, "Mode", GLUT_BITMAP_HELVETICA_12, 0.75f, 0.80f, 0.88f);
    if (g_traffic->isEmergencyActive()) {
        drawText(130, 306, "EMERGENCY", GLUT_BITMAP_HELVETICA_12, 1.0f, 0.2f, 0.25f);
    } else {
        drawText(145, 306, "Normal", GLUT_BITMAP_HELVETICA_12, 0.4f, 0.8f, 0.95f);
    }

    drawText(24, 285, "COA ALU", GLUT_BITMAP_HELVETICA_10, 0.5f, 0.6f, 0.7f);
    drawText(140, 285, "x86 ASM", GLUT_BITMAP_HELVETICA_10, 1.0f, 0.85f, 0.2f);

    // Syllabus Card (NEP 2023)
    drawCard(14, 65, sbW - 14, 260, 0.07f, 0.10f, 0.16f, 0.15f, 0.20f, 0.30f);
    drawText(20, 240, "MITAOE Syllabus", GLUT_BITMAP_HELVETICA_12, 0.95f, 0.85f, 0.25f);
    drawText(20, 222, "Second Year B.Tech CE", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.65f, 0.75f);
    drawText(20, 198, "1. CG  (2310218T/L)", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.90f, 0.95f);
    drawText(20, 180, "2. OOP (2310261L)", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.90f, 0.95f);
    drawText(20, 162, "3. DSA (2310212T/L)", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.90f, 0.95f);
    drawText(20, 144, "4. COA (2310221T/L)", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.90f, 0.95f);
    drawText(20, 118, "All Course Outcomes", GLUT_BITMAP_HELVETICA_10, 0.40f, 0.80f, 0.95f);
    drawText(20, 100, "(CO1-CO6) Covered", GLUT_BITMAP_HELVETICA_10, 0.40f, 0.80f, 0.95f);

    drawText(18, 28, "Smart City Infrastructure Lab", GLUT_BITMAP_HELVETICA_10, 0.40f, 0.46f, 0.55f);
}

// ------------------------------------------------------------------------------
// VIEW 0: LANDING HUB (TAB_LANDING)
// ------------------------------------------------------------------------------
static void renderLandingView() {
    float startX = 250.0f;

    // Intelligent Transport System Badge
    drawRect(startX, 668, startX + 225, 694, 0.10f, 0.20f, 0.38f);
    drawRectOutline(startX, 668, startX + 225, 694, 0.22f, 0.45f, 0.85f);
    drawBresenhamCircle(startX + 14, 681, 4.0f, 0.2f, 0.6f, 1.0f);
    drawText(startX + 26, 676, "Intelligent Transport System", GLUT_BITMAP_HELVETICA_12, 0.35f, 0.70f, 1.0f);

    drawText(startX, 625, "Smart Traffic & Emergency Response Simulator", GLUT_BITMAP_TIMES_ROMAN_24, 0.95f, 0.98f, 1.0f);

    drawText(startX, 588, "A high-fidelity academic simulation of a smart city junction grid, where an", GLUT_BITMAP_HELVETICA_12, 0.68f, 0.74f, 0.82f);
    drawText(startX, 570, "emergency detection engine overrides traffic patterns using priority queues,", GLUT_BITMAP_HELVETICA_12, 0.68f, 0.74f, 0.82f);
    drawText(startX, 552, "stacks, and real-time vehicle analytics with real x86 Assembly hardware logic.", GLUT_BITMAP_HELVETICA_12, 0.68f, 0.74f, 0.82f);

    // Hero Action Buttons
    drawInteractiveButton(20, startX, 495, 175, 40, "> Launch Simulator", 0.15f, 0.42f, 0.92f);
    drawInteractiveButton(21, startX + 190, 495, 175, 40, "Explore DSA Lab", 0.12f, 0.16f, 0.25f, false, true);

    // Hero Intersection Preview Card
    float heroX = 720.0f, heroY = 460.0f, heroW = 445.0f, heroH = 220.0f;
    drawCard(heroX, heroY, heroX + heroW, heroY + heroH, 0.07f, 0.10f, 0.16f, 0.18f, 0.25f, 0.38f);

    float mcx = heroX + heroW * 0.5f;
    float mcy = heroY + heroH * 0.5f + 15.0f;
    drawRect(heroX + 15, mcy - 24, heroX + heroW - 15, mcy + 24, 0.18f, 0.20f, 0.28f);
    drawRect(mcx - 24, heroY + 45, mcx + 24, heroY + heroH - 15, 0.18f, 0.20f, 0.28f);

    drawClippedLine(heroX + 20, mcy, heroX + heroW - 20, mcy, heroX, heroY, heroX + heroW, heroY + heroH, 0.9f, 0.9f, 0.9f, 1.2f);
    drawClippedLine(mcx, heroY + 50, mcx, heroY + heroH - 20, heroX, heroY, heroX + heroW, heroY + heroH, 0.9f, 0.9f, 0.9f, 1.2f);

    drawBresenhamCircle(mcx, mcy, 18.0f, 0.1f, 0.15f, 0.25f, true);
    drawBresenhamCircle(mcx, mcy, 18.0f, 0.2f, 0.55f, 0.95f, false);

    drawRect(mcx - 65, mcy - 8, mcx - 45, mcy + 6, 0.2f, 0.6f, 0.95f);
    drawRect(mcx + 45, mcy - 9, mcx + 75, mcy + 8, 0.95f, 0.75f, 0.15f);

    float heroAmbPulse = 1.0f + 0.12f * sinf(g_tick * 0.2f);
    drawRect(mcx - 10 * heroAmbPulse, mcy + 35, mcx + 10 * heroAmbPulse, mcy + 55, 0.95f, 0.15f, 0.20f);

    drawRect(heroX + 15, heroY + 12, heroX + 140, heroY + 36, 0.12f, 0.16f, 0.24f);
    drawText(heroX + 35, heroY + 20, "4 Junctions", GLUT_BITMAP_HELVETICA_10, 0.4f, 0.75f, 1.0f);

    drawRect(heroX + 155, heroY + 12, heroX + 290, heroY + 36, 0.12f, 0.16f, 0.24f);
    drawText(heroX + 175, heroY + 20, "Real-time Sim", GLUT_BITMAP_HELVETICA_10, 0.2f, 0.95f, 0.5f);

    drawRect(heroX + 305, heroY + 12, heroX + 430, heroY + 36, 0.12f, 0.16f, 0.24f);
    drawText(heroX + 330, heroY + 20, "4 Subjects", GLUT_BITMAP_HELVETICA_10, 0.1f, 0.85f, 0.95f);

    // 4 System Status Cards
    drawText(startX, 435, "System Status", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);

    float cardW = 215.0f, cardH = 80.0f, cardY = 335.0f;

    drawCard(startX, cardY, startX + cardW, cardY + cardH);
    drawRect(startX + 14, cardY + 20, startX + 54, cardY + 60, 0.12f, 0.35f, 0.22f);
    drawText(startX + 27, cardY + 35, "V", GLUT_BITMAP_HELVETICA_18, 0.2f, 0.95f, 0.45f);
    drawText(startX + 65, cardY + 45, std::to_string(g_traffic->getVehicles().size()), GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(startX + 65, cardY + 26, "Active Vehicles", GLUT_BITMAP_HELVETICA_12, 0.55f, 0.62f, 0.72f);

    float c2X = startX + cardW + 15.0f;
    drawCard(c2X, cardY, c2X + cardW, cardY + cardH);
    drawRect(c2X + 14, cardY + 20, c2X + 54, cardY + 60, 0.12f, 0.25f, 0.45f);
    drawText(c2X + 27, cardY + 35, "J", GLUT_BITMAP_HELVETICA_18, 0.3f, 0.7f, 1.0f);
    drawText(c2X + 65, cardY + 45, "4", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(c2X + 65, cardY + 26, "Junctions Online", GLUT_BITMAP_HELVETICA_12, 0.55f, 0.62f, 0.72f);

    float c3X = c2X + cardW + 15.0f;
    drawCard(c3X, cardY, c3X + cardW, cardY + cardH);
    drawRect(c3X + 14, cardY + 20, c3X + 54, cardY + 60, 0.45f, 0.15f, 0.20f);
    drawText(c3X + 27, cardY + 35, "!", GLUT_BITMAP_HELVETICA_18, 1.0f, 0.3f, 0.35f);
    drawText(c3X + 65, cardY + 45, std::to_string(g_traffic->getEmergencyCount()), GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(c3X + 65, cardY + 26, "Emergency Events", GLUT_BITMAP_HELVETICA_12, 0.55f, 0.62f, 0.72f);

    float c4X = c3X + cardW + 15.0f;
    drawCard(c4X, cardY, c4X + cardW, cardY + cardH);
    drawRect(c4X + 14, cardY + 20, c4X + 54, cardY + 60, 0.10f, 0.32f, 0.38f);
    drawText(c4X + 27, cardY + 35, "T", GLUT_BITMAP_HELVETICA_18, 0.15f, 0.85f, 0.95f);
    drawText(c4X + 65, cardY + 45, std::to_string(g_traffic->getUptime()) + "s", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(c4X + 65, cardY + 26, "Simulation Uptime", GLUT_BITMAP_HELVETICA_12, 0.55f, 0.62f, 0.72f);

    // Curriculum Course Outcomes (CO) Matrix
    drawText(startX, 290, "MITAOE Academic Curriculum Course Outcomes (CO) Matrix:", GLUT_BITMAP_HELVETICA_12, 0.95f, 0.85f, 0.25f);

    float coW = 215.0f, coH = 225.0f, coY = 45.0f;

    drawCard(startX, coY, startX + coW, coY + coH, 0.08f, 0.11f, 0.17f, 0.22f, 0.35f, 0.55f);
    drawText(startX + 12, coY + 200, "1. Computer Graphics", GLUT_BITMAP_HELVETICA_12, 0.35f, 0.75f, 1.0f);
    drawText(startX + 12, coY + 182, "(2310218T / 2310218L)", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.65f, 0.75f);
    drawText(startX + 12, coY + 158, "* CO1: OpenGL Primitives", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(startX + 12, coY + 138, "* CO2: 2D Scale/Rot/Trans", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(startX + 12, coY + 118, "* CO3: 2D Objects & Roads", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(startX + 12, coY + 98, "* CO4: Ortho Projection", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(startX + 12, coY + 78, "* CO5: Cohen-Sutherland Clip", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(startX + 12, coY + 58, "* CO6: Texture & Light Glow", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);

    drawCard(c2X, coY, c2X + coW, coY + coH, 0.08f, 0.11f, 0.17f, 0.22f, 0.45f, 0.35f);
    drawText(c2X + 12, coY + 200, "2. OOP using C++", GLUT_BITMAP_HELVETICA_12, 0.35f, 0.95f, 0.55f);
    drawText(c2X + 12, coY + 182, "(Course: 2310261L)", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.65f, 0.75f);
    drawText(c2X + 12, coY + 158, "* CO1: Vehicle/Signal Class", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c2X + 12, coY + 138, "* CO2: Inheritance: Base ->", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c2X + 22, coY + 120, "Car, Bus, Ambulance", GLUT_BITMAP_HELVETICA_10, 0.75f, 0.85f, 0.90f);
    drawText(c2X + 12, coY + 100, "* CO3: Runtime Polymorphism", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c2X + 22, coY + 82, "virtual draw(), update()", GLUT_BITMAP_HELVETICA_10, 0.75f, 0.85f, 0.90f);
    drawText(c2X + 12, coY + 60, "* CO4: Smart Pointers STL", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);

    drawCard(c3X, coY, c3X + coW, coY + coH, 0.08f, 0.11f, 0.17f, 0.45f, 0.35f, 0.20f);
    drawText(c3X + 12, coY + 200, "3. Data Structures", GLUT_BITMAP_HELVETICA_12, 1.0f, 0.80f, 0.25f);
    drawText(c3X + 12, coY + 182, "(2310212T / 2310214L)", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.65f, 0.75f);
    drawText(c3X + 12, coY + 158, "* CO1-2: Array Registry", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c3X + 12, coY + 138, "* CO3: Singly Linked Queue", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c3X + 12, coY + 118, "* CO4: Priority Queue", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c3X + 22, coY + 100, "& Event Stack (LIFO)", GLUT_BITMAP_HELVETICA_10, 0.75f, 0.85f, 0.90f);
    drawText(c3X + 12, coY + 80, "* CO5: Bubble Sort & Search", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c3X + 12, coY + 60, "* CO6: Hash Table & Probe", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);

    drawCard(c4X, coY, c4X + coW, coY + coH, 0.08f, 0.11f, 0.17f, 0.45f, 0.20f, 0.35f);
    drawText(c4X + 12, coY + 200, "4. Computer Org & Arch", GLUT_BITMAP_HELVETICA_12, 1.0f, 0.40f, 0.75f);
    drawText(c4X + 12, coY + 182, "(2310221T / 2310221L)", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.65f, 0.75f);
    drawText(c4X + 12, coY + 158, "* CO1: 80386 x86 Registers", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c4X + 12, coY + 138, "* CO2: Real NASM Assembly", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c4X + 22, coY + 120, "(COA_Logic.asm linked)", GLUT_BITMAP_HELVETICA_10, 0.75f, 0.85f, 0.90f);
    drawText(c4X + 12, coY + 100, "* CO3: Arithmetic Operations", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
    drawText(c4X + 22, coY + 82, "CMP, IMUL, ADD, TEST", GLUT_BITMAP_HELVETICA_10, 0.75f, 0.85f, 0.90f);
    drawText(c4X + 12, coY + 60, "* CO4: Control Signal Logic", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.88f, 0.95f);
}

// ------------------------------------------------------------------------------
// CG CO3: Traffic Signal Post
// ------------------------------------------------------------------------------
static void renderTrafficSignal(float x, float y, int road, int activeLane, int emergencyLane) {
    int state = g_traffic->getSignalState(road);
    int timerSec = g_traffic->getSignalTimer(road);
    bool isGreen = (state == 2 || road == emergencyLane);
    bool isYellow = (state == 1 && road != emergencyLane);
    bool isRed = (!isGreen && !isYellow);

    drawRect(x - 12, y - 30, x + 12, y + 30, 0.10f, 0.12f, 0.16f);
    drawRectOutline(x - 12, y - 30, x + 12, y + 30, 0.35f, 0.42f, 0.52f, 1.2f);

    float rLit = isRed ? 1.0f : 0.18f;
    drawBresenhamCircle(x, y + 19, 5.5f, rLit, 0.05f, 0.05f, true);
    if (isRed) drawLightGlow(x, y + 19, 14.0f, 1.0f, 0.1f, 0.1f, 0.45f);

    float yLit = isYellow ? 1.0f : 0.18f;
    drawBresenhamCircle(x, y, 5.5f, yLit, yLit * 0.85f, 0.05f, true);
    if (isYellow) drawLightGlow(x, y, 14.0f, 1.0f, 0.85f, 0.1f, 0.45f);

    float gLit = isGreen ? 1.0f : 0.15f;
    drawBresenhamCircle(x, y - 19, 5.5f, 0.05f, gLit, 0.1f, true);
    if (isGreen) drawLightGlow(x, y - 19, 14.0f, 0.1f, 1.0f, 0.3f, 0.55f);

    if (timerSec > 0 || isGreen) {
        drawRect(x - 11, y + 33, x + 11, y + 47, 0.08f, 0.10f, 0.15f);
        drawRectOutline(x - 11, y + 33, x + 11, y + 47, 0.25f, 0.35f, 0.45f);
        std::string tStr = std::to_string(timerSec) + "s";
        drawText(x - 8, y + 36, tStr, GLUT_BITMAP_HELVETICA_10, isGreen ? 0.2f : (isYellow ? 1.0f : 0.95f), isGreen ? 1.0f : (isYellow ? 0.9f : 0.3f), 0.2f);
    }
}

// ------------------------------------------------------------------------------
// CG CO2: Vehicle Rendering with Transformations
// ------------------------------------------------------------------------------
static void renderAllVehicles(float mapX1, float mapY1, float mapX2, float mapY2) {
    float cx = (mapX1 + mapX2) * 0.5f;
    float cy = (mapY1 + mapY2) * 0.5f;

    const auto& vehicles = g_traffic->getVehicles();

    for (const auto& v : vehicles) {
        int road = v->getRoad();
        float pos = v->getPosition();
        float vx = cx, vy = cy;
        float angle = 0.0f;

        if (road == EAST) {
            vx = mapX2 - pos * (mapX2 - mapX1);
            vy = cy - 25.0f;
            angle = 180.0f;
        } else if (road == WEST) {
            vx = mapX1 + pos * (mapX2 - mapX1);
            vy = cy + 25.0f;
            angle = 0.0f;
        } else if (road == SOUTH) {
            vx = cx - 25.0f;
            vy = mapY1 + pos * (mapY2 - mapY1);
            angle = 90.0f;
        } else if (road == NORTH) {
            vx = cx + 25.0f;
            vy = mapY2 - pos * (mapY2 - mapY1);
            angle = 270.0f;
        }

        glPushMatrix();
        glTranslatef(vx, vy, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        if (v->getType() == "Ambulance" && g_traffic->isEmergencyActive()) {
            glScalef(g_pulse, g_pulse, 1.0f);
        }

        v->draw();
        glPopMatrix();
    }
}

// ------------------------------------------------------------------------------
// VIEW 1: LIVE DASHBOARD (TAB_DASHBOARD)
// ------------------------------------------------------------------------------
static void renderDashboardView() {
    float startX = 250.0f;

    drawText(startX, 688, "Live Traffic Dashboard", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(startX, 668, "Command center — junction states & emergency controls (MITAOE Smart City Grid)", GLUT_BITMAP_HELVETICA_12, 0.55f, 0.62f, 0.72f);

    // Top action buttons
    drawInteractiveButton(30, 780, 665, 130, 36, "Open Registry", 0.18f, 0.24f, 0.35f, false, true);

    float emPulse = 0.85f + 0.15f * sinf(g_tick * 0.25f);
    drawInteractiveButton(31, 925, 665, 235, 36, "! Detect Emergency", 0.90f * emPulse, 0.15f, 0.22f);

    // --------------------------------------------------------------------------
    // LEFT: SIMULATION MAP (x: 250 to 805, y: 195 to 645)
    // --------------------------------------------------------------------------
    float mapX1 = 250.0f, mapY1 = 195.0f, mapX2 = 805.0f, mapY2 = 645.0f;
    drawCard(mapX1, mapY1, mapX2, mapY2, 0.06f, 0.09f, 0.15f, 0.18f, 0.24f, 0.35f);

    drawText(mapX1 + 14, mapY2 - 24, "Simulation Map", GLUT_BITMAP_HELVETICA_12, 0.90f, 0.92f, 0.98f);
    float livePulse = 0.5f + 0.5f * sinf(g_tick * 0.15f);
    drawBresenhamCircle(mapX2 - 60, mapY2 - 18, 4.0f, 0.2f * livePulse, 0.95f * livePulse, 0.4f * livePulse, true);
    drawText(mapX2 - 50, mapY2 - 23, "LIVE", GLUT_BITMAP_HELVETICA_10, 0.2f, 0.95f, 0.45f);

    float jx1 = mapX1 + 10.0f, jy1 = mapY1 + 35.0f, jx2 = mapX2 - 10.0f, jy2 = mapY2 - 35.0f;
    float cx = (jx1 + jx2) * 0.5f;
    float cy = (jy1 + jy2) * 0.5f;
    float rw = 52.0f;

    drawRect(jx1, jy1, jx2, jy2, 0.12f, 0.28f, 0.16f);
    drawRect(jx1, cy - rw, jx2, cy + rw, 0.18f, 0.20f, 0.25f);
    drawRect(cx - rw, jy1, cx + rw, jy2, 0.18f, 0.20f, 0.25f);

    for (int x = (int)jx1 + 10; x < cx - rw - 15; x += 30)
        drawRect(x, cy - 1.5f, x + 18, cy + 1.5f, 0.95f, 0.85f, 0.2f);
    for (int x = (int)(cx + rw + 15); x < jx2 - 10; x += 30)
        drawRect(x, cy - 1.5f, x + 18, cy + 1.5f, 0.95f, 0.85f, 0.2f);
    for (int y = (int)jy1 + 10; y < cy - rw - 15; y += 30)
        drawRect(cx - 1.5f, y, cx + 1.5f, y + 18, 0.95f, 0.85f, 0.2f);
    for (int y = (int)(cy + rw + 15); y < jy2 - 10; y += 30)
        drawRect(cx - 1.5f, y, cx + 1.5f, y + 18, 0.95f, 0.85f, 0.2f);

    for (float y = cy - rw + 4; y < cy + rw - 4; y += 12) {
        drawRect(cx - rw - 12, y, cx - rw - 3, y + 6, 0.95f, 0.95f, 0.95f);
        drawRect(cx + rw + 3, y, cx + rw + 12, y + 6, 0.95f, 0.95f, 0.95f);
    }
    for (float x = cx - rw + 4; x < cx + rw - 4; x += 12) {
        drawRect(x, cy - rw - 12, x + 6, cy - rw - 3, 0.95f, 0.95f, 0.95f);
        drawRect(x, cy + rw + 3, x + 6, cy + rw + 12, 0.95f, 0.95f, 0.95f);
    }

    int emergency = g_traffic->getEmergency();
    if (emergency != -1) {
        float pulse = 0.35f + 0.20f * sinf(g_tick * 0.3f);
        if (emergency == EAST) drawRect(cx + rw, cy - rw, jx2, cy + rw, 0.1f * pulse, 0.9f * pulse, 0.3f * pulse);
        else if (emergency == WEST) drawRect(jx1, cy - rw, cx - rw, cy + rw, 0.1f * pulse, 0.9f * pulse, 0.3f * pulse);
        else if (emergency == SOUTH) drawRect(cx - rw, jy1, cx + rw, cy - rw, 0.1f * pulse, 0.9f * pulse, 0.3f * pulse);
        else if (emergency == NORTH) drawRect(cx - rw, cy + rw, cx + rw, jy2, 0.1f * pulse, 0.9f * pulse, 0.3f * pulse);

        drawRect(jx1 + 10, jy2 - 32, jx2 - 10, jy2 - 4, 0.85f, 0.12f, 0.18f);
        drawRectOutline(jx1 + 10, jy2 - 32, jx2 - 10, jy2 - 4, 1.0f, 0.9f, 0.2f, 1.8f);
        const char* rNames[4] = {"EAST", "WEST", "SOUTH", "NORTH"};
        std::string alertMsg = "! EMERGENCY PREEMPTION: AMBULANCE ON " + std::string(rNames[emergency]) + " ROAD (GREEN CORRIDOR ACTIVE) !";
        drawText(jx1 + 20, jy2 - 22, alertMsg, GLUT_BITMAP_HELVETICA_12, 1.0f, 1.0f, 1.0f);
    }

    int green = g_traffic->getGreen();
    renderTrafficSignal(cx + rw + 22, cy + rw + 16, EAST, green, emergency);
    renderTrafficSignal(cx - rw - 22, cy + rw + 16, NORTH, green, emergency);
    renderTrafficSignal(cx - rw - 22, cy - rw - 16, WEST, green, emergency);
    renderTrafficSignal(cx + rw + 22, cy - rw - 16, SOUTH, green, emergency);

    renderAllVehicles(jx1, jy1, jx2, jy2);

    drawText(jx2 - 65, cy + rw + 6, "EAST [0]", GLUT_BITMAP_HELVETICA_10, 0.8f, 0.85f, 0.95f);
    drawText(jx1 + 10, cy + rw + 6, "WEST [1]", GLUT_BITMAP_HELVETICA_10, 0.8f, 0.85f, 0.95f);
    drawText(cx + rw + 6, jy1 + 15, "SOUTH [2]", GLUT_BITMAP_HELVETICA_10, 0.8f, 0.85f, 0.95f);
    drawText(cx - rw - 60, jy2 - 15, "NORTH [3]", GLUT_BITMAP_HELVETICA_10, 0.8f, 0.85f, 0.95f);

    float legY = mapY1 + 10.0f;
    drawRect(mapX1 + 15, legY, mapX1 + 27, legY + 12, 0.20f, 0.55f, 0.95f);
    drawText(mapX1 + 32, legY + 2, "Car", GLUT_BITMAP_HELVETICA_10, 0.7f, 0.75f, 0.85f);

    drawRect(mapX1 + 80, legY, mapX1 + 96, legY + 12, 0.95f, 0.75f, 0.15f);
    drawText(mapX1 + 102, legY + 2, "Bus", GLUT_BITMAP_HELVETICA_10, 0.7f, 0.75f, 0.85f);

    drawRect(mapX1 + 150, legY, mapX1 + 166, legY + 12, 0.95f, 0.15f, 0.20f);
    drawText(mapX1 + 172, legY + 2, "Emergency", GLUT_BITMAP_HELVETICA_10, 0.7f, 0.75f, 0.85f);

    drawBresenhamCircle(mapX1 + 265, legY + 6, 5.0f, 0.1f, 0.95f, 0.3f, true);
    drawText(mapX1 + 275, legY + 2, "Green Signal", GLUT_BITMAP_HELVETICA_10, 0.7f, 0.75f, 0.85f);

    drawBresenhamCircle(mapX1 + 365, legY + 6, 5.0f, 0.95f, 0.1f, 0.1f, true);
    drawText(mapX1 + 375, legY + 2, "Red Signal", GLUT_BITMAP_HELVETICA_10, 0.7f, 0.75f, 0.85f);

    // --------------------------------------------------------------------------
    // RIGHT: JUNCTION CONTROLS & COA TELEMETRY (x: 820 to 1160, y: 195 to 645)
    // --------------------------------------------------------------------------
    float ctrlX1 = 820.0f, ctrlW = 340.0f;
    drawCard(ctrlX1, mapY1, ctrlX1 + ctrlW, mapY2, 0.07f, 0.10f, 0.17f, 0.18f, 0.24f, 0.35f);

    drawText(ctrlX1 + 15, mapY2 - 25, "Junction Controls", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);

    // 4 Road junction toggle rows
    float rRowY = mapY2 - 62.0f;
    const char* dNames[4] = {"East Crossing", "West Avenue", "South Boulevard", "North Highway"};
    int btnGreenIds[4] = {41, 42, 43, 44};

    for (int i = 0; i < 4; ++i) {
        int sigState = g_traffic->getSignalState(i);
        bool isSigGreen = (sigState == 2 || i == emergency);
        bool isSigYellow = (sigState == 1 && i != emergency);

        drawCard(ctrlX1 + 12, rRowY, ctrlX1 + ctrlW - 12, rRowY + 34, 0.09f, 0.13f, 0.20f, 0.16f, 0.22f, 0.32f);

        drawText(ctrlX1 + 22, rRowY + 18, dNames[i], GLUT_BITMAP_HELVETICA_10, 0.90f, 0.92f, 0.98f);
        std::string qInfo = "Lane Queue: " + std::to_string(g_traffic->getQueue(i));
        drawText(ctrlX1 + 22, rRowY + 6, qInfo, GLUT_BITMAP_HELVETICA_10, 0.50f, 0.60f, 0.75f);

        std::string stateTxt = isSigGreen ? "GREEN" : (isSigYellow ? "YELLOW" : "RED");
        float btnR = isSigGreen ? 0.12f : (isSigYellow ? 0.85f : 0.85f);
        float btnG = isSigGreen ? 0.75f : (isSigYellow ? 0.65f : 0.15f);
        float btnB = isSigGreen ? 0.25f : (isSigYellow ? 0.10f : 0.18f);

        drawInteractiveButton(btnGreenIds[i], ctrlX1 + ctrlW - 85, rRowY + 5, 68, 24, stateTxt, btnR, btnG, btnB);

        rRowY -= 40.0f;
    }

    // Emergency Ambulance trigger buttons
    drawText(ctrlX1 + 15, rRowY + 5, "Trigger Ambulance Road Preemption:", GLUT_BITMAP_HELVETICA_12, 1.0f, 0.85f, 0.25f);
    rRowY -= 32.0f;
    drawInteractiveButton(32, ctrlX1 + 12, rRowY, (ctrlW - 30) * 0.5f, 26, "Amb North", 0.85f, 0.15f, 0.20f);
    drawInteractiveButton(33, ctrlX1 + 18 + (ctrlW - 30) * 0.5f, rRowY, (ctrlW - 30) * 0.5f, 26, "Amb South", 0.85f, 0.15f, 0.20f);
    rRowY -= 30.0f;
    drawInteractiveButton(34, ctrlX1 + 12, rRowY, (ctrlW - 30) * 0.5f, 26, "Amb East", 0.85f, 0.15f, 0.20f);
    drawInteractiveButton(35, ctrlX1 + 18 + (ctrlW - 30) * 0.5f, rRowY, (ctrlW - 30) * 0.5f, 26, "Amb West", 0.85f, 0.15f, 0.20f);

    // Spawning & Control Buttons (includes big "Spawn Vehicle" button like website!)
    rRowY -= 34.0f;
    drawInteractiveButton(40, ctrlX1 + 12, rRowY, ctrlW - 24, 28, "+ Spawn Vehicle", 0.15f, 0.45f, 0.85f);

    rRowY -= 32.0f;
    drawInteractiveButton(36, ctrlX1 + 12, rRowY, (ctrlW - 30) * 0.5f, 26, "+ Add Car", 0.18f, 0.28f, 0.45f, false, true);
    drawInteractiveButton(37, ctrlX1 + 18 + (ctrlW - 30) * 0.5f, rRowY, (ctrlW - 30) * 0.5f, 26, "+ Add Bus", 0.85f, 0.60f, 0.15f, false, true);

    rRowY -= 30.0f;
    drawInteractiveButton(38, ctrlX1 + 12, rRowY, (ctrlW - 30) * 0.5f, 26, "Next Phase", 0.35f, 0.25f, 0.65f);
    std::string pauseTxt = g_traffic->getPaused() ? "RESUME" : "PAUSE";
    drawInteractiveButton(39, ctrlX1 + 18 + (ctrlW - 30) * 0.5f, rRowY, (ctrlW - 30) * 0.5f, 26, pauseTxt, 0.35f, 0.38f, 0.45f);

    // COA Hardware Assembly Telemetry Card
    rRowY -= 75.0f;
    drawCard(ctrlX1 + 12, rRowY, ctrlX1 + ctrlW - 12, rRowY + 68, 0.05f, 0.08f, 0.14f, 0.25f, 0.35f, 0.50f);
    drawText(ctrlX1 + 20, rRowY + 50, "COA x86 Assembly Telemetry (COA_Logic.asm):", GLUT_BITMAP_HELVETICA_10, 1.0f, 0.90f, 0.30f);

    std::string coaPStr = "* check_traffic_priority() = " + std::to_string(g_traffic->getCoaPriority()) + " (ALU CMP vehicleCount > 2)";
    drawText(ctrlX1 + 20, rRowY + 32, coaPStr, GLUT_BITMAP_HELVETICA_10, 0.85f, 0.90f, 0.95f);

    std::string coaTStr = "* calculate_green_time()  = " + std::to_string(g_traffic->getCoaTime()) + "s (Dynamic allocation)";
    drawText(ctrlX1 + 20, rRowY + 16, coaTStr, GLUT_BITMAP_HELVETICA_10, 0.85f, 0.90f, 0.95f);

    // --------------------------------------------------------------------------
    // BOTTOM: ACTION HISTORY (TRAFFICSTACK - LIFO) (x: 250 to 1160, y: 25 to 180)
    // --------------------------------------------------------------------------
    float histX = 250.0f, histY = 25.0f, histW = 910.0f, histH = 155.0f;
    drawCard(histX, histY, histX + histW, histY + histH, 0.07f, 0.10f, 0.16f, 0.18f, 0.24f, 0.35f);

    drawText(histX + 15, histY + histH - 22, "Action History (TrafficStack - LIFO Data Structure)", GLUT_BITMAP_HELVETICA_12, 0.95f, 0.95f, 1.0f);

    drawInteractiveButton(45, histX + histW - 200, histY + histH - 30, 90, 22, "Pop (Undo)", 0.25f, 0.30f, 0.42f, false, true);
    drawInteractiveButton(46, histX + histW - 100, histY + histH - 30, 85, 22, "Reset Grid", 0.45f, 0.15f, 0.20f, false, true);

    std::vector<std::string> events = g_traffic->getStack().getAllEvents();
    float evY = histY + histH - 48.0f;
    if (events.empty()) {
        drawText(histX + 25, histY + 60, "No actions recorded yet.", GLUT_BITMAP_HELVETICA_12, 0.45f, 0.50f, 0.60f);
    } else {
        int maxShow = std::min((int)events.size(), 5);
        for (int i = 0; i < maxShow; ++i) {
            float bulletR = (i == 0) ? 0.2f : 0.4f;
            float bulletG = (i == 0) ? 0.95f : 0.5f;
            float bulletB = (i == 0) ? 0.4f : 0.6f;
            drawBresenhamCircle(histX + 25, evY + 4, 3.5f, bulletR, bulletG, bulletB, true);
            drawText(histX + 38, evY, events[i], GLUT_BITMAP_HELVETICA_12, (i == 0) ? 0.95f : 0.70f, (i == 0) ? 0.98f : 0.75f, (i == 0) ? 1.0f : 0.82f);
            evY -= 21.0f;
        }
    }
}

// ------------------------------------------------------------------------------
// VIEW 2: VEHICLE REGISTRY (TAB_REGISTRY)
// ------------------------------------------------------------------------------
static void renderRegistryView() {
    float startX = 250.0f;

    drawText(startX, 688, "Vehicle Management Registry", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(startX, 668, "All simulated vehicle instances — search, sort & manage (DSA Array & Search/Sort)", GLUT_BITMAP_HELVETICA_12, 0.55f, 0.62f, 0.72f);

    const auto& reg = g_traffic->getRegistry();
    int total = (int)reg.size();
    int emergCount = 0;
    float sumP = 0;
    for (const auto& r : reg) {
        if (r.type == "Ambulance") ++emergCount;
        sumP += r.priority;
    }
    float avgP = total > 0 ? (sumP / total) : 0.0f;

    float stW = 295.0f, stH = 68.0f, stY = 585.0f;

    drawCard(startX, stY, startX + stW, stY + stH);
    drawText(startX + 18, stY + 44, "Total Registered", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.62f, 0.72f);
    drawText(startX + 18, stY + 16, std::to_string(total), GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);

    float st2X = startX + stW + 15.0f;
    drawCard(st2X, stY, st2X + stW, stY + stH);
    drawText(st2X + 18, stY + 44, "Emergency Vehicles", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.62f, 0.72f);
    drawText(st2X + 18, stY + 16, std::to_string(emergCount), GLUT_BITMAP_HELVETICA_18, 1.0f, 0.25f, 0.30f);

    float st3X = st2X + stW + 15.0f;
    drawCard(st3X, stY, st3X + stW, stY + stH);
    drawText(st3X + 18, stY + 44, "Avg Priority Score", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.62f, 0.72f);
    char buf[16];
    snprintf(buf, sizeof(buf), "%.1f", avgP);
    drawText(st3X + 18, stY + 16, buf, GLUT_BITMAP_HELVETICA_18, 0.15f, 0.85f, 0.95f);

    // Filter & Sort Controls
    float barY = 535.0f;
    drawText(startX, barY + 10, "Sort Table:", GLUT_BITMAP_HELVETICA_12, 0.85f, 0.90f, 0.95f);

    drawInteractiveButton(50, startX + 85, barY, 110, 28, "By Priority", 0.20f, 0.30f, 0.45f, g_registrySortMode == 0, g_registrySortMode != 0);
    drawInteractiveButton(51, startX + 205, barY, 110, 28, "By Spawn Time", 0.20f, 0.30f, 0.45f, g_registrySortMode == 1, g_registrySortMode != 1);
    drawInteractiveButton(52, startX + 325, barY, 100, 28, "By Type", 0.20f, 0.30f, 0.45f, g_registrySortMode == 2, g_registrySortMode != 2);
    drawInteractiveButton(53, startX + 435, barY, 90, 28, "By ID", 0.20f, 0.30f, 0.45f, g_registrySortMode == 3, g_registrySortMode != 3);

    drawInteractiveButton(54, startX + 540, barY, 80, 28, "+ Car", 0.15f, 0.45f, 0.85f);
    drawInteractiveButton(55, startX + 630, barY, 80, 28, "+ Bus", 0.85f, 0.60f, 0.15f);
    drawInteractiveButton(56, startX + 720, barY, 100, 28, "+ Ambulance", 0.85f, 0.15f, 0.20f);
    drawInteractiveButton(57, startX + 830, barY, 85, 28, "Clear Cleared", 0.30f, 0.35f, 0.45f, false, true);

    // Table Card
    float tblX = startX, tblY = 45.0f, tblW = 915.0f, tblH = 475.0f;
    drawCard(tblX, tblY, tblX + tblW, tblY + tblH, 0.07f, 0.10f, 0.16f, 0.18f, 0.24f, 0.35f);

    float hY = tblY + tblH - 32.0f;
    drawRect(tblX + 5, hY - 8, tblX + tblW - 5, hY + 22, 0.09f, 0.13f, 0.22f);
    drawClippedLine(tblX + 5, hY - 8, tblX + tblW - 5, hY - 8, tblX, tblY, tblX + tblW, tblY + tblH, 0.20f, 0.25f, 0.35f);

    drawText(tblX + 25, hY, "ID", GLUT_BITMAP_HELVETICA_10, 0.60f, 0.68f, 0.78f);
    drawText(tblX + 110, hY, "TYPE", GLUT_BITMAP_HELVETICA_10, 0.60f, 0.68f, 0.78f);
    drawText(tblX + 240, hY, "ROAD", GLUT_BITMAP_HELVETICA_10, 0.60f, 0.68f, 0.78f);
    drawText(tblX + 370, hY, "PRIORITY", GLUT_BITMAP_HELVETICA_10, 0.60f, 0.68f, 0.78f);
    drawText(tblX + 500, hY, "SPAWNED", GLUT_BITMAP_HELVETICA_10, 0.60f, 0.68f, 0.78f);
    drawText(tblX + 630, hY, "STATUS", GLUT_BITMAP_HELVETICA_10, 0.60f, 0.68f, 0.78f);
    drawText(tblX + 790, hY, "ACTION", GLUT_BITMAP_HELVETICA_10, 0.60f, 0.68f, 0.78f);

    float rowY = hY - 38.0f;
    const char* roadNames[4] = {"East", "West", "South", "North"};

    int showCount = std::min((int)reg.size(), 10);
    for (int i = 0; i < showCount; ++i) {
        const auto& item = reg[i];
        bool isEm = (item.type == "Ambulance");
        bool isCleared = (item.status == "Cleared");

        if (i % 2 == 1) {
            drawRect(tblX + 5, rowY - 6, tblX + tblW - 5, rowY + 24, 0.08f, 0.11f, 0.18f);
        }

        drawText(tblX + 25, rowY + 4, "#" + std::to_string(item.id), GLUT_BITMAP_HELVETICA_12, 0.90f, 0.95f, 1.0f);

        float badgeR = isEm ? 0.85f : (item.type == "Bus" ? 0.85f : 0.15f);
        float badgeG = isEm ? 0.15f : (item.type == "Bus" ? 0.60f : 0.45f);
        float badgeB = isEm ? 0.20f : (item.type == "Bus" ? 0.15f : 0.90f);
        drawRect(tblX + 105, rowY - 2, tblX + 205, rowY + 18, badgeR * 0.25f, badgeG * 0.25f, badgeB * 0.25f);
        drawRectOutline(tblX + 105, rowY - 2, tblX + 205, rowY + 18, badgeR, badgeG, badgeB, 1.0f);
        drawText(tblX + 115, rowY + 2, item.type, GLUT_BITMAP_HELVETICA_10, badgeR, badgeG, badgeB);

        drawText(tblX + 240, rowY + 4, roadNames[item.road % 4], GLUT_BITMAP_HELVETICA_12, 0.75f, 0.82f, 0.90f);

        std::string pStr = (item.priority == 2) ? "2 (High)" : ((item.priority == 1) ? "1 (Medium)" : "0 (Normal)");
        drawText(tblX + 370, rowY + 4, pStr, GLUT_BITMAP_HELVETICA_12, isEm ? 1.0f : 0.85f, isEm ? 0.3f : 0.90f, isEm ? 0.3f : 0.95f);

        drawText(tblX + 500, rowY + 4, "+" + std::to_string(item.spawnSecond) + "s", GLUT_BITMAP_HELVETICA_12, 0.60f, 0.68f, 0.78f);

        if (isCleared) {
            drawText(tblX + 630, rowY + 4, "Cleared", GLUT_BITMAP_HELVETICA_12, 0.45f, 0.52f, 0.62f);
            drawText(tblX + 795, rowY + 4, "Done", GLUT_BITMAP_HELVETICA_10, 0.40f, 0.45f, 0.55f);
        } else {
            drawText(tblX + 630, rowY + 4, "Active", GLUT_BITMAP_HELVETICA_12, 0.20f, 0.95f, 0.45f);
            drawInteractiveButton(1000 + item.id, tblX + 785, rowY - 2, 75, 22, "Remove", 0.85f, 0.20f, 0.25f, false, true);
        }

        rowY -= 36.0f;
    }

    std::string summary = "Showing " + std::to_string(showCount) + " of " + std::to_string(total) + " registered vehicles · Sorted by " + (g_registrySortMode == 0 ? "Priority" : (g_registrySortMode == 1 ? "Time" : (g_registrySortMode == 2 ? "Type" : "ID")));
    drawText(tblX + 20, tblY + 12, summary, GLUT_BITMAP_HELVETICA_10, 0.50f, 0.58f, 0.68f);
}

// ------------------------------------------------------------------------------
// VIEW 3: DSA LABORATORY (TAB_DSA_LAB)
// ------------------------------------------------------------------------------
static void renderDsaLabView() {
    float startX = 250.0f;

    drawText(startX, 688, "DSA Laboratory", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(startX, 668, "Interactive data structure visualizations powering the simulation (DSA CO1-CO6)", GLUT_BITMAP_HELVETICA_12, 0.55f, 0.62f, 0.72f);

    float boxW = 445.0f, boxH = 300.0f;

    // 1. Stack Visualizer (CO4)
    float sX = startX, sY = 350.0f;
    drawCard(sX, sY, sX + boxW, sY + boxH);

    drawText(sX + 16, sY + boxH - 24, "Stack (LIFO - Last In First Out)", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(sX + 16, sY + boxH - 42, "Used for action history, signal undo log, and backtracking.", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.62f, 0.72f);

    float scX = sX + 16.0f, scY = sY + 50.0f, scW = boxW - 32.0f, scH = 195.0f;
    drawRect(scX, scY, scX + scW, scY + scH, 0.05f, 0.08f, 0.14f);
    drawRectOutline(scX, scY, scX + scW, scY + scH, 0.18f, 0.24f, 0.35f);

    std::vector<std::string> stackItems = g_traffic->getStack().getAllEvents();
    if (stackItems.empty()) {
        drawText(scX + scW * 0.5f - 85, scY + scH * 0.5f, "Stack is empty — push an event.", GLUT_BITMAP_HELVETICA_10, 0.45f, 0.50f, 0.60f);
    } else {
        int maxDraw = std::min((int)stackItems.size(), 5);
        float elemY = scY + 12.0f;
        for (int i = maxDraw - 1; i >= 0; --i) {
            bool isTop = (i == 0);
            float br = isTop ? 0.15f : 0.12f;
            float bg = isTop ? 0.45f : 0.18f;
            float bb = isTop ? 0.92f : 0.28f;

            drawRect(scX + 10, elemY, scX + scW - 10, elemY + 28, br, bg, bb);
            drawRectOutline(scX + 10, elemY, scX + scW - 10, elemY + 28, isTop ? 0.4f : 0.2f, isTop ? 0.7f : 0.3f, isTop ? 1.0f : 0.45f);

            drawText(scX + 20, elemY + 8, stackItems[i].substr(0, 42), GLUT_BITMAP_HELVETICA_10, 1.0f, 1.0f, 1.0f);
            if (isTop) {
                drawRect(scX + scW - 65, elemY + 4, scX + scW - 16, elemY + 24, 0.85f, 0.15f, 0.20f);
                drawText(scX + scW - 55, elemY + 8, "TOP", GLUT_BITMAP_HELVETICA_10, 1.0f, 1.0f, 1.0f);
            }
            elemY += 34.0f;
        }
    }

    drawInteractiveButton(70, sX + 16, sY + 14, 100, 26, "Push Signal", 0.15f, 0.42f, 0.92f);
    drawInteractiveButton(71, sX + 125, sY + 14, 90, 26, "Pop (Undo)", 0.20f, 0.25f, 0.38f, false, true);
    drawInteractiveButton(72, sX + 225, sY + 14, 80, 26, "Clear", 0.45f, 0.15f, 0.20f, false, true);
    std::string szStr = "Size: " + std::to_string(g_traffic->getStack().size());
    drawText(sX + 320, sY + 20, szStr, GLUT_BITMAP_HELVETICA_12, 0.2f, 0.95f, 0.45f);

    // 2. Priority Queue Visualizer (CO4)
    float qX = startX + boxW + 20.0f, qY = 350.0f;
    drawCard(qX, qY, qX + boxW, qY + boxH);

    drawText(qX + 16, qY + boxH - 24, "Priority Queue (Ambulance Preemption)", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(qX + 16, qY + boxH - 42, "Ambulance (Priority 2) jumps straight to the head of the queue.", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.62f, 0.72f);

    float qcX = qX + 16.0f, qcY = qY + 50.0f, qcW = boxW - 32.0f, qcH = 195.0f;
    drawRect(qcX, qcY, qcX + qcW, qcY + qcH, 0.05f, 0.08f, 0.14f);
    drawRectOutline(qcX, qcY, qcX + qcW, qcY + qcH, 0.18f, 0.24f, 0.35f);

    const auto& pqItems = g_traffic->getPriorityQueue().getItems();
    if (pqItems.empty()) {
        drawText(qcX + qcW * 0.5f - 95, qcY + qcH * 0.5f, "Queue is empty — enqueue a vehicle.", GLUT_BITMAP_HELVETICA_10, 0.45f, 0.50f, 0.60f);
    } else {
        float qElemX = qcX + 10.0f;
        int maxDraw = std::min((int)pqItems.size(), 4);
        for (int i = 0; i < maxDraw; ++i) {
            const auto& it = pqItems[i];
            bool isEm = (it.priority == 2);
            float br = isEm ? 0.85f : 0.15f;
            float bg = isEm ? 0.15f : 0.45f;
            float bb = isEm ? 0.20f : 0.85f;

            drawCard(qElemX, qcY + 25.0f, qElemX + 90.0f, qcY + 165.0f, br * 0.25f, bg * 0.25f, bb * 0.25f, br, bg, bb);
            drawText(qElemX + 12, qcY + 140, it.type, GLUT_BITMAP_HELVETICA_10, br, bg, bb);
            drawText(qElemX + 12, qcY + 115, "#" + std::to_string(it.id), GLUT_BITMAP_HELVETICA_12, 1.0f, 1.0f, 1.0f);
            drawText(qElemX + 12, qcY + 90, "P: " + std::to_string(it.priority), GLUT_BITMAP_HELVETICA_10, 1.0f, 0.85f, 0.2f);
            if (i == 0) {
                drawRect(qElemX + 8, qcY + 45, qElemX + 82, qcY + 68, 0.12f, 0.38f, 0.22f);
                drawText(qElemX + 22, qcY + 52, "HEAD", GLUT_BITMAP_HELVETICA_10, 0.2f, 0.95f, 0.45f);
            }
            qElemX += 98.0f;
        }
    }

    drawInteractiveButton(73, qX + 16, qY + 14, 85, 26, "+ Enqueue Car", 0.15f, 0.42f, 0.92f);
    drawInteractiveButton(74, qX + 108, qY + 14, 85, 26, "+ Enqueue Bus", 0.85f, 0.60f, 0.15f);
    drawInteractiveButton(75, qX + 200, qY + 14, 85, 26, "+ Enqueue Amb", 0.85f, 0.15f, 0.20f);
    drawInteractiveButton(76, qX + 292, qY + 14, 75, 26, "Dequeue", 0.20f, 0.25f, 0.38f, false, true);
    std::string qSzStr = "Sz: " + std::to_string(g_traffic->getPriorityQueue().size());
    drawText(qX + 375, qY + 20, qSzStr, GLUT_BITMAP_HELVETICA_12, 0.2f, 0.95f, 0.45f);

    // 3. Vehicle Hash Table (CO6)
    float hX = startX, hY = 30.0f;
    drawCard(hX, hY, hX + boxW, hY + boxH);

    drawText(hX + 16, hY + boxH - 24, "Vehicle Hash Table (CO6 - Modulo & Probing)", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(hX + 16, hY + boxH - 42, "Formula: hash(id) = id % 10 with linear probing collision resolution.", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.62f, 0.72f);

    float hcX = hX + 16.0f, hcY = hY + 50.0f;
    for (int i = 0; i < 10; ++i) {
        float bX = hcX + (i % 5) * 82.0f;
        float bY = (i < 5) ? (hcY + 105.0f) : (hcY + 30.0f);

        int val = g_traffic->getHash().getBucket(i);
        std::string typ = g_traffic->getHash().getType(i);
        bool occupied = (val > 0);

        drawCard(bX, bY, bX + 76.0f, bY + 65.0f, occupied ? 0.10f : 0.05f, occupied ? 0.15f : 0.08f, occupied ? 0.24f : 0.12f);
        drawText(bX + 6, bY + 48, "[" + std::to_string(i) + "]", GLUT_BITMAP_HELVETICA_10, 0.45f, 0.55f, 0.65f);

        if (occupied) {
            drawText(bX + 6, bY + 28, "#" + std::to_string(val), GLUT_BITMAP_HELVETICA_12, 0.2f, 0.95f, 0.45f);
            drawText(bX + 6, bY + 10, typ, GLUT_BITMAP_HELVETICA_10, 0.8f, 0.85f, 0.95f);
        } else if (val == -2) {
            drawText(bX + 6, bY + 28, "[Del]", GLUT_BITMAP_HELVETICA_10, 0.85f, 0.35f, 0.2f);
        } else {
            drawText(bX + 6, bY + 28, "Empty", GLUT_BITMAP_HELVETICA_10, 0.35f, 0.40f, 0.48f);
        }
    }

    drawInteractiveButton(77, hX + 16, hY + 14, 115, 26, "+ Insert Key", 0.15f, 0.42f, 0.92f);
    drawInteractiveButton(78, hX + 140, hY + 14, 110, 26, "Search #102", 0.20f, 0.25f, 0.38f, false, true);
    drawInteractiveButton(79, hX + 260, hY + 14, 75, 26, "Clear", 0.45f, 0.15f, 0.20f, false, true);
    drawText(hX + 16, hY + boxH - 62, g_hashStatusMsg, GLUT_BITMAP_HELVETICA_10, 0.3f, 0.9f, 0.95f);

    // 4. Searching & Sorting Lab (CO5)
    float sortX = startX + boxW + 20.0f, sortY = 30.0f;
    drawCard(sortX, sortY, sortX + boxW, sortY + boxH);

    drawText(sortX + 16, sortY + boxH - 24, "Searching & Sorting (CO5 - Bubble Sort)", GLUT_BITMAP_HELVETICA_18, 0.95f, 0.95f, 1.0f);
    drawText(sortX + 16, sortY + boxH - 42, "Real-time visual comparison and swapping of vehicle identification tags.", GLUT_BITMAP_HELVETICA_10, 0.55f, 0.62f, 0.72f);

    float bcX = sortX + 20.0f, bcY = sortY + 70.0f;
    for (int i = 0; i < 6; ++i) {
        int v = g_sortArray[i];
        float barH = (v - 95) * 11.0f;
        float bX = bcX + i * 68.0f;

        bool isComparing = (!g_sortDone && (i == g_sortJ || i == g_sortJ + 1));
        float r = isComparing ? 0.95f : 0.15f;
        float g = isComparing ? 0.45f : 0.55f;
        float b = isComparing ? 0.15f : 0.92f;

        drawCard(bX, bcY, bX + 54.0f, bcY + barH, r * 0.3f, g * 0.3f, b * 0.3f, r, g, b);
        drawText(bX + 10, bcY + 8, "#" + std::to_string(v), GLUT_BITMAP_HELVETICA_10, 1.0f, 1.0f, 1.0f);
        drawText(bX + 18, bcY - 18, "[" + std::to_string(i) + "]", GLUT_BITMAP_HELVETICA_10, 0.5f, 0.6f, 0.7f);
    }

    drawInteractiveButton(80, sortX + 16, sortY + 14, 110, 26, "Sort Step", 0.15f, 0.42f, 0.92f);
    drawInteractiveButton(81, sortX + 135, sortY + 14, 115, 26, "Linear Search", 0.20f, 0.25f, 0.38f, false, true);
    drawInteractiveButton(82, sortX + 260, sortY + 14, 85, 26, "Shuffle", 0.35f, 0.25f, 0.65f, false, true);
    drawText(sortX + 16, sortY + boxH - 62, g_sortStatusMsg, GLUT_BITMAP_HELVETICA_10, 1.0f, 0.85f, 0.25f);
}

// ------------------------------------------------------------------------------
// Main Display Function
// ------------------------------------------------------------------------------
static void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    g_activeButtons.clear();

    // Render Sidebar Navigation
    renderSidebar();

    // Render Active View
    switch (g_currentTab) {
        case TAB_LANDING:
            renderLandingView();
            break;
        case TAB_DASHBOARD:
            renderDashboardView();
            break;
        case TAB_REGISTRY:
            renderRegistryView();
            break;
        case TAB_DSA_LAB:
            renderDsaLabView();
            break;
    }

    glutSwapBuffers();
}

// ------------------------------------------------------------------------------
// Timer Callback (~30 FPS animation & state update)
// ------------------------------------------------------------------------------
static void timer(int) {
    ++g_tick;

    g_pulse += g_pulseStep;
    if (g_pulse >= 1.25f) {
        g_pulse = 1.25f;
        g_pulseStep = -0.02f;
    } else if (g_pulse <= 1.0f) {
        g_pulse = 1.0f;
        g_pulseStep = 0.02f;
    }

    if (g_traffic) {
        g_traffic->update();
    }

    glutPostRedisplay();
    glutTimerFunc(33, timer, 0);
}

// ------------------------------------------------------------------------------
// Mouse Click Callback - Guaranteed coordinate scaling for High-DPI and Resizing!
// ------------------------------------------------------------------------------
static void mouseClick(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    // Convert physical screen window pixels to virtual 1200 x 720 orthographic coordinates
    int winW = (g_windowWidth > 0) ? g_windowWidth : 1200;
    int winH = (g_windowHeight > 0) ? g_windowHeight : 720;
    float glX = ((float)x / (float)winW) * 1200.0f;
    float glY = ((float)(winH - y) / (float)winH) * 720.0f;

    for (const auto& btn : g_activeButtons) {
        if (glX >= btn.x && glX <= btn.x + btn.w && glY >= btn.y && glY <= btn.y + btn.h) {
            // Sidebar Navigation Tabs
            if (btn.id == 10) g_currentTab = TAB_LANDING;
            else if (btn.id == 11) g_currentTab = TAB_DASHBOARD;
            else if (btn.id == 12) g_currentTab = TAB_REGISTRY;
            else if (btn.id == 13) g_currentTab = TAB_DSA_LAB;

            // Landing Actions
            else if (btn.id == 20) g_currentTab = TAB_DASHBOARD;
            else if (btn.id == 21) g_currentTab = TAB_DSA_LAB;

            // Dashboard Actions
            else if (btn.id == 30) g_currentTab = TAB_REGISTRY;
            else if (btn.id == 31) {
                // Find most congested lane or default to North
                int maxQ = -1, bestRoad = NORTH;
                for (int r = 0; r < 4; ++r) {
                    if (g_traffic->getQueue(r) > maxQ) {
                        maxQ = g_traffic->getQueue(r);
                        bestRoad = r;
                    }
                }
                g_traffic->triggerEmergency(bestRoad);
            }
            else if (btn.id == 32) g_traffic->triggerEmergency(NORTH);
            else if (btn.id == 33) g_traffic->triggerEmergency(SOUTH);
            else if (btn.id == 34) g_traffic->triggerEmergency(EAST);
            else if (btn.id == 35) g_traffic->triggerEmergency(WEST);
            else if (btn.id == 36) g_traffic->addCar(rand() % 4);
            else if (btn.id == 37) g_traffic->addBus(rand() % 4);
            else if (btn.id == 38) g_traffic->nextPhase();
            else if (btn.id == 39) g_traffic->togglePause();
            else if (btn.id == 40) g_traffic->spawnRandomVehicle();
            else if (btn.id >= 41 && btn.id <= 44) {
                // Manual junction traffic light toggle
                int road = btn.id - 41;
                g_traffic->setManualGreen(road);
            }
            else if (btn.id == 45) {
                std::string popped;
                g_traffic->getStack().pop(popped);
            }
            else if (btn.id == 46) {
                g_traffic->reset();
            }

            // Registry Actions
            else if (btn.id == 50) {
                g_registrySortMode = 0;
                g_traffic->sortRegistry(0);
            }
            else if (btn.id == 51) {
                g_registrySortMode = 1;
                g_traffic->sortRegistry(1);
            }
            else if (btn.id == 52) {
                g_registrySortMode = 2;
                g_traffic->sortRegistry(2);
            }
            else if (btn.id == 53) {
                g_registrySortMode = 3;
                g_traffic->sortRegistry(3);
            }
            else if (btn.id == 54) g_traffic->addCar(rand() % 4);
            else if (btn.id == 55) g_traffic->addBus(rand() % 4);
            else if (btn.id == 56) g_traffic->triggerEmergency(rand() % 4);
            else if (btn.id == 57) g_traffic->clearClearedRegistry();
            else if (btn.id >= 1000) {
                int vId = btn.id - 1000;
                g_traffic->removeVehicleById(vId);
            }

            // DSA Lab Actions
            else if (btn.id == 70) {
                static int pushSeq = 1;
                g_traffic->getStack().push("Signal Override #" + std::to_string(pushSeq++));
            }
            else if (btn.id == 71) {
                std::string popped;
                g_traffic->getStack().pop(popped);
            }
            else if (btn.id == 72) {
                g_traffic->getStack().clear();
            }
            else if (btn.id == 73) {
                static int pqCarId = 201;
                g_traffic->getPriorityQueue().insert(pqCarId++, rand() % 4, 0, "Car");
            }
            else if (btn.id == 74) {
                static int pqBusId = 301;
                g_traffic->getPriorityQueue().insert(pqBusId++, rand() % 4, 1, "Bus");
            }
            else if (btn.id == 75) {
                static int pqAmbId = 901;
                g_traffic->getPriorityQueue().insert(pqAmbId++, rand() % 4, 2, "Ambulance");
            }
            else if (btn.id == 76) {
                EmergencyItem item;
                g_traffic->getPriorityQueue().popHighest(item);
            }
            else if (btn.id == 77) {
                int rId = 100 + rand() % 900;
                bool ins = g_traffic->getHash().insert(rId, "Vehicle");
                g_hashStatusMsg = ins ? ("Inserted ID #" + std::to_string(rId) + " at bucket [" + std::to_string(rId % 10) + "]") : "Hash full or duplicate!";
            }
            else if (btn.id == 78) {
                int idx = g_traffic->getHash().search(102);
                g_hashStatusMsg = (idx != -1) ? ("ID #102 found at Bucket [" + std::to_string(idx) + "] (O(1))") : "ID #102 not found";
            }
            else if (btn.id == 79) {
                g_traffic->getHash().clear();
                g_hashStatusMsg = "Hash table cleared.";
            }
            else if (btn.id == 80) {
                if (g_sortI < 5) {
                    if (g_sortArray[g_sortJ] > g_sortArray[g_sortJ + 1]) {
                        int temp = g_sortArray[g_sortJ];
                        g_sortArray[g_sortJ] = g_sortArray[g_sortJ + 1];
                        g_sortArray[g_sortJ + 1] = temp;
                        g_sortStatusMsg = "Swapped #" + std::to_string(g_sortArray[g_sortJ + 1]) + " and #" + std::to_string(g_sortArray[g_sortJ]);
                    } else {
                        g_sortStatusMsg = "Comparing #" + std::to_string(g_sortArray[g_sortJ]) + " <= #" + std::to_string(g_sortArray[g_sortJ + 1]) + " (In order)";
                    }
                    ++g_sortJ;
                    if (g_sortJ >= 5 - g_sortI) {
                        g_sortJ = 0;
                        ++g_sortI;
                    }
                } else {
                    g_sortDone = true;
                    g_sortStatusMsg = "Bubble Sort Complete! Array fully sorted.";
                }
            }
            else if (btn.id == 81) {
                int target = 104;
                int pos = -1;
                for (int i = 0; i < 6; ++i) {
                    if (g_sortArray[i] == target) { pos = i; break; }
                }
                g_sortStatusMsg = (pos != -1) ? ("Linear Search: ID #" + std::to_string(target) + " found at Index [" + std::to_string(pos) + "]") : "Not found";
            }
            else if (btn.id == 82) {
                g_sortArray[0] = 105; g_sortArray[1] = 101; g_sortArray[2] = 104;
                g_sortArray[3] = 102; g_sortArray[4] = 106; g_sortArray[5] = 103;
                g_sortI = 0; g_sortJ = 0; g_sortDone = false;
                g_sortStatusMsg = "Array shuffled. Ready for sorting/searching.";
            }

            break;
        }
    }
    glutPostRedisplay();
}

static void mouseHover(int x, int y) {
    int winW = (g_windowWidth > 0) ? g_windowWidth : 1200;
    int winH = (g_windowHeight > 0) ? g_windowHeight : 720;
    g_mouseX = (int)(((float)x / (float)winW) * 1200.0f);
    g_mouseY = (int)(((float)(winH - y) / (float)winH) * 720.0f);
}

// ------------------------------------------------------------------------------
// Keyboard Callback
// ------------------------------------------------------------------------------
static void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '\t':
            g_currentTab = (ViewTab)((g_currentTab + 1) % 4);
            break;
        case 'h':
        case 'H':
            g_currentTab = TAB_LANDING;
            break;
        case 'd':
        case 'D':
            g_currentTab = TAB_DASHBOARD;
            break;
        case 'v':
        case 'V':
            g_currentTab = TAB_REGISTRY;
            break;
        case 'l':
        case 'L':
            g_currentTab = TAB_DSA_LAB;
            break;
        case '1':
            g_traffic->triggerEmergency(EAST);
            break;
        case '2':
            g_traffic->triggerEmergency(WEST);
            break;
        case '3':
            g_traffic->triggerEmergency(SOUTH);
            break;
        case '4':
            g_traffic->triggerEmergency(NORTH);
            break;
        case 'c':
        case 'C':
            g_traffic->addCar(rand() % 4);
            break;
        case 'b':
        case 'B':
            g_traffic->addBus(rand() % 4);
            break;
        case 'a':
        case 'A':
            g_traffic->triggerEmergency(rand() % 4);
            break;
        case ' ':
            g_traffic->togglePause();
            break;
        case 'n':
        case 'N':
            g_traffic->nextPhase();
            break;
        case 'r':
        case 'R':
            g_traffic->reset();
            break;
        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}

// Window resize handler (CG CO4: Viewport & Projection Transformation)
static void reshape(int w, int h) {
    g_windowWidth = (w > 0) ? w : 1200;
    g_windowHeight = (h > 0) ? h : 720;
    glViewport(0, 0, g_windowWidth, g_windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 720); // Virtual resolution scaling
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ------------------------------------------------------------------------------
// Main Graphics Entry Point
// ------------------------------------------------------------------------------
void runGraphics(TrafficController& controller) {
    g_traffic = &controller;

    int argc = 1;
    char appName[] = "Smart Traffic Management & Emergency Simulator";
    char* argv[] = {appName, nullptr};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 720);
    glutInitWindowPosition(60, 40);

    glutCreateWindow("Smart Traffic & Emergency Response Simulator - MITAOE (NEP 2023)");

    glClearColor(0.04f, 0.06f, 0.11f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 720);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseHover);
    glutMotionFunc(mouseHover);
    glutTimerFunc(33, timer, 0);

    glutMainLoop();
}
