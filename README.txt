================================================================================
SMART TRAFFIC MANAGEMENT AND EMERGENCY RESPONSE SIMULATOR
MIT Academy of Engineering, Alandi (Pune) - Second Year B.Tech (NEP 2023)
================================================================================

1. PROJECT SUMMARY & UI ARCHITECTURE
--------------------------------------------------------------------------------
This project is a high-fidelity academic simulation of a smart city junction grid
with emergency ambulance priority preemption. Recreated entirely using Computer
Graphics (OpenGL/FreeGLUT) with NO external web libraries, the application renders
the complete modern multi-view dashboard interface inspired by:
https://b-011391a3-4c79-4014-b146-e62a0dc842db-q8ek6qbkgvezm6mx.ui2code.ai/

The UI includes:
  - Left Dark Navy Sidebar with brand logo, online status LED, and live vehicle count.
  - Tab 0: "Landing Hub" — Hero section, system status cards, and academic syllabus matrix.
  - Tab 1: "Live Dashboard" — Simulation map, 3-lens signals with timers, emergency corridor,
           junction controls, COA telemetry, and LIFO action history.
  - Tab 2: "Vehicle Registry" — Instance registry table with sorting (Priority, Time, Type, ID),
           status badges, and interactive vehicle removal.
  - Tab 3: "DSA Laboratory" — Interactive Stack (LIFO), Priority Queue (FIFO by Priority),
           Vehicle Hash Table (modulo + linear probing), and Bubble Sort & Linear Search.

--------------------------------------------------------------------------------
2. FULL ACADEMIC COURSE OUTCOMES (CO) INTEGRATION MATRIX
--------------------------------------------------------------------------------
ALL Course Outcomes from the 4 technical subjects of MITAOE Second Year are mapped:

1. COMPUTER GRAPHICS (2310218T / 2310218L):
   - CO1: OpenGL 2D graphics primitives (GL_QUADS, GL_LINES, GL_TRIANGLE_FAN, Bitmap fonts).
   - CO2: Geometric transformations:
          * Translation (glTranslatef) for vehicle movement and UI positioning.
          * Rotation (glRotatef) for vehicle orientation (0, 90, 180, 270 deg).
          * Scaling (glScalef) for ambulance beacon pulsing and UI responsive elements.
   - CO3: 2D objects design & algorithms (Bresenham/Midpoint circle for signals & dials,
          4-way crossroad geometry, zebra pedestrian crosswalks).
   - CO4: Orthographic projection using gluOrtho2D(0, 1200, 0, 720) and viewport scaling.
   - CO5: Clipping algorithms (Cohen-Sutherland Line Clipping for road boundaries,
          glScissor viewport clipping for cards and lists).
   - CO6: Colors & light simulation (diffused point-source lens glow, RGB color models,
          pulsing emergency strobes).

2. PROBLEM SOLVING USING OOP (2310261L):
   - CO1: Classes, constructors, destructors (Vehicle, TrafficSignal, TrafficController).
   - CO2: Inheritance: Abstract base class Vehicle -> Derived Car, Bus, and Ambulance.
   - CO3: Runtime polymorphism with pure virtual functions draw() and update().
   - CO4: Real-world problem solving with modular controller, STL containers, and smart pointers.

3. DATA STRUCTURES (2310212T / 2310214L):
   - CO1: Linear Data Structures: Array-based approach lane structures.
   - CO2: Concept of Arrays: Vehicle Registry table array representation.
   - CO3: Singly Linked List Queue: FIFO queue managing each road approach lane.
   - CO4: Stack & Priority Queue:
          * TrafficStack (LIFO) for action history and signal change undo logs.
          * EmergencyPriorityQueue (Priority 2 Ambulance preemption).
   - CO5: Searching & Sorting: Bubble Sort step-by-step & Linear Search visualizer in DSA Lab.
   - CO6: Hashing: VehicleHash table with hash(id) = id % 10 and linear probing collision resolution.

4. COMPUTER ORGANIZATION & ARCHITECTURE (2310221T / 2310221L):
   - CO1: 80386/x86_64 architecture and register set (RAX, RCX, RDX).
   - CO2: Real Assembly Language Programming in COA_Logic.asm (assembled with NASM).
   - CO3: Processor-level arithmetic operations (CMP subtraction, IMUL, ADD, TEST flags).
   - CO4: Hardware control signal generation and dynamic priority decision logic:
          * check_traffic_priority() evaluates lane congestion against threshold.
          * calculate_green_time() dynamically allocates signal green durations.
          * coa_evaluate_sensor() evaluates bitwise hardware triggers.
          * Real assembly linkage via extern "C" in C++.

--------------------------------------------------------------------------------
3. CONTROLS (MOUSE & KEYBOARD)
--------------------------------------------------------------------------------
A. Mouse Clicking:
   - Sidebar Tabs: Click [Landing Hub], [Live Dashboard], [Vehicle Registry], [DSA Laboratory]
   - Dashboard:
     * Click [! Detect Emergency] -> Triggers priority ambulance on busiest lane
     * Click [Amb North/South/East/West] -> Triggers ambulance on specific road
     * Click [+ Add Car] / [+ Add Bus] -> Spawns vehicles
     * Click [Next Phase] -> Advances traffic light cycle
     * Click [PAUSE / RESUME] -> Freezes/unfreezes simulation
     * Click [Pop (Undo)] -> Pops latest event from TrafficStack
     * Click [Reset Grid] -> Resets simulation grid
   - Registry:
     * Click [By Priority], [By Spawn Time], [By Type], [By ID] to sort table
     * Click [Remove] on any row to clear that vehicle
   - DSA Lab:
     * Click [Push Signal] / [Pop (Undo)] / [Clear] on Stack
     * Click [+ Enqueue Car] / [+ Enqueue Amb] / [Dequeue Head] on Priority Queue
     * Click [+ Insert Key] / [Search #102] on Vehicle Hash Table
     * Click [Sort Step] / [Linear Search] / [Shuffle] on Searching & Sorting Lab

B. Keyboard Shortcuts:
   - Tab = Cycle through tabs (Landing -> Dashboard -> Registry -> DSA Lab)
   - H = Landing Hub | D = Live Dashboard | V = Vehicle Registry | L = DSA Lab
   - 1 = East Emergency | 2 = West Emergency | 3 = South Emergency | 4 = North Emergency
   - C = Add Car | B = Add Bus | A = Add Ambulance
   - Space = Pause/Resume | N = Next Phase | R = Reset Grid | Esc = Exit

--------------------------------------------------------------------------------
4. COMPILATION & RUN
--------------------------------------------------------------------------------
Using MSYS2 MinGW 64-bit in Windows PowerShell:

   $env:PATH = "C:\msys64\mingw64\bin;C:\msys64\usr\bin;" + $env:PATH
   cd "D:\SmartTrafficManagementEmergencySimulator (2)\SmartTrafficManagementEmergencySimulator"
   .\build.bat

Or manually:
   nasm -f win64 COA_Logic.asm -o COA_Logic.o
   g++ -std=c++17 -Wall main.cpp DS.cpp OOP.cpp CG.cpp COA_Logic.o -o SmartTrafficSimulator.exe -lfreeglut -lopengl32 -lglu32
   .\SmartTrafficSimulator.exe
================================================================================
