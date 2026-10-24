SMART TRAFFIC MANAGEMENT AND EMERGENCY SIMULATOR

Files:
- main.cpp: application entry point
- DS.h / DS.cpp: linked-list vehicle queues
- OOP.h / OOP.cpp: controller, signal cycle and emergency override
- CG.h / CG.cpp: OpenGL / FreeGLUT graphics
- COA.asm: separate 8086/MASM-style COA example

Normal signal sequence: East -> West -> South -> North -> repeat.
Press E to trigger an emergency on the next road.
Press 1=East, 2=West, 3=South, 4=North for a specific emergency.
Press Esc to exit.

Requirements: Windows, VS Code, MSYS2 MINGW64, FreeGLUT.

Install in the MSYS2 MINGW64 terminal:
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-freeglut

Compile from this folder:
g++ -std=c++17 main.cpp DS.cpp OOP.cpp CG.cpp -o SmartTrafficEmergencySimulator.exe -lfreeglut -lopengl32 -lglu32

Run:
./SmartTrafficEmergencySimulator.exe

COA.asm is a separate assembly demonstration and is not linked into the C++ executable.
