// ==============================================================================
// SMART TRAFFIC MANAGEMENT & EMERGENCY RESPONSE SIMULATOR
// MIT Academy of Engineering, Alandi (Pune) - Second Year B.Tech (NEP 2023)
//
// Integrated Subjects:
//   1. Computer Graphics (2310218T / 2310218L)
//   2. Problem Solving using OOP (2310261L)
//   3. Data Structures (2310212T / 2310214L)
//   4. Computer Organization & Architecture (2310221T / 2310221L)
// ==============================================================================

#include "OOP.h"
#include "CG.h"
#include <iostream>

int main() {
    std::cout << "===============================================================\n";
    std::cout << "     SMART TRAFFIC & EMERGENCY RESPONSE SIMULATOR\n";
    std::cout << "     MIT Academy of Engineering, Alandi (MITAOE)\n";
    std::cout << "===============================================================\n";
    std::cout << " Academic Course Outcomes (CO) Integration:\n";
    std::cout << "  - CG  : OpenGL 2D Primitives, Transformations, Traffic Junction\n";
    std::cout << "  - OOP : Vehicle Base Class, Car/Bus/Ambulance Polymorphism\n";
    std::cout << "  - DSA : Singly-Linked Queue (FIFO), Priority Queue, Event Stack\n";
    std::cout << "  - COA : Real x86 Assembly (COA_Logic.asm) linked via extern 'C'\n";
    std::cout << "---------------------------------------------------------------\n";
    std::cout << " Starting Simulator Window (1200x720) - Modern Dashboard Interface...\n" << std::flush;

    TrafficController controller;
    runGraphics(controller);

    std::cout << "Simulator closed successfully.\n";
    return 0;
}
