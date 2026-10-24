#ifndef CG_H
#define CG_H

#include "OOP.h"

// Active view tabs matching the web dashboard layout
enum ViewTab {
    TAB_LANDING = 0,
    TAB_DASHBOARD = 1,
    TAB_REGISTRY = 2,
    TAB_DSA_LAB = 3
};

// Initializes and starts the OpenGL / FreeGLUT graphical interface
void runGraphics(TrafficController& controller);

#endif // CG_H
