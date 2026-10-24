#include "VehicleManager.h"

int main() {
    VehicleManager manager;

    manager.addVehicle(101, "Car", "East");
    manager.addVehicle(102, "Ambulance", "North");
    manager.addVehicle(103, "Bus", "West");

    manager.displayVehicles();

    return 0;
}