@echo off
echo ====================================================================
echo  Compiling Smart Traffic Management Simulator (MITAOE Project)
echo ====================================================================
set PATH=C:\msys64\mingw64\bin;C:\msys64\usr\bin;%PATH%

echo [1/2] Assembling COA Assembly Module (COA_Logic.asm)...
nasm -f win64 COA_Logic.asm -o COA_Logic.o
if %ERRORLEVEL% NEQ 0 (
    echo Error in assembly compilation!
    pause
    exit /b %ERRORLEVEL%
)

echo [2/2] Compiling C++ and Linking with Assembly Object...
g++ -std=c++17 -Wall main.cpp DS.cpp OOP.cpp CG.cpp COA_Logic.o -o SmartTrafficSimulator.exe -lfreeglut -lopengl32 -lglu32
if %ERRORLEVEL% NEQ 0 (
    echo Error in C++ compilation!
    pause
    exit /b %ERRORLEVEL%
)

copy SmartTrafficSimulator.exe SmartTrafficEmergencySimulator.exe /Y >nul
echo Build successful! Generated SmartTrafficSimulator.exe
pause
