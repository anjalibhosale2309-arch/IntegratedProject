#include "OOP.h"
#include "CG.h"
#include "DSA.h"
#include <iostream>
using namespace std;

int main() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << " SMART TRAFFIC MANAGEMENT SIMULATOR\n";
        cout << "====================================\n";
        cout << "1. Run Traffic Graphics Simulation\n";
        cout << "2. Run DSA Modules\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            TrafficController controller;
            cout << "E = emergency on next road\n";
            cout << "1 = East, 2 = West, 3 = South, 4 = North\n";
            runGraphics(controller);
            break;
        }
        case 2:
            runDSADemo();
            break;
        case 0:
            cout << "Exiting Smart Traffic Simulator.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
