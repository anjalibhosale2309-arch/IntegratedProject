
#include "DSA.h"
#include "VehicleManager.h"
#include "TrafficStack.h"
#include "SearchSort.h"
#include "VehicleHash.h"

#include <iostream>
using namespace std;

void runDSADemo() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << " SMART TRAFFIC - DSA MODULES\n";
        cout << "====================================\n";
        cout << "1. Vehicle Management (Array)\n";
        cout << "2. Traffic Event History (Stack)\n";
        cout << "3. Search and Sort Vehicle IDs\n";
        cout << "4. Vehicle ID Hashing\n";
        cout << "0. Exit DSA Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            VehicleManager manager;

            manager.addVehicle(101, "Car", "East");
            manager.addVehicle(102, "Ambulance", "North");
            manager.addVehicle(103, "Bus", "West");

            manager.displayVehicles();
            break;
        }

        case 2: {
            TrafficStack history;

            history.push("East lane: Normal traffic");
            history.push("North lane: Ambulance detected");
            history.push("North lane: Emergency signal activated");

            history.display();
            break;
        }

        case 3: {
            int vehicleIDs[] = {105, 101, 104, 102, 103};
            int n = 5;
            int key;

            cout << "Enter Vehicle ID to search: ";
            cin >> key;

            linearSearch(vehicleIDs, n, key);
            bubbleSort(vehicleIDs, n);
            break;
        }

        case 4: {
            VehicleHash vehicles;

            vehicles.insert(101);
            vehicles.insert(102);
            vehicles.insert(111);

            vehicles.display();

            int key;
            cout << "Enter Vehicle ID to search: ";
            cin >> key;

            vehicles.search(key);
            break;
        }

        case 0:
            cout << "Exiting DSA Menu.\n";
            break;

        default:
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);
}
