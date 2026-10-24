
#include "VehicleManager.h"
#include <iostream>
using namespace std;

VehicleManager::VehicleManager() {
    count = 0;
}

bool VehicleManager::addVehicle(int id, string type, string direction) {
    if (count >= MAX_VEHICLES) {
        cout << "Vehicle list is full!\n";
        return false;
    }

    vehicles[count].id = id;
    vehicles[count].type = type;
    vehicles[count].direction = direction;
    count++;

    return true;
}

void VehicleManager::displayVehicles() const {
    cout << "\n--- Vehicle Management ---\n";

    if (count == 0) {
        cout << "No vehicles available.\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        cout << "ID: " << vehicles[i].id
             << " | Type: " << vehicles[i].type
             << " | Direction: " << vehicles[i].direction
             << '\n';
    }
}
