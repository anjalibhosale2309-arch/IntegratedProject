#include "VehicleHash.h"

int main() {
    VehicleHash vehicles;

    vehicles.insert(101);
    vehicles.insert(102);
    vehicles.insert(111);

    vehicles.display();

    vehicles.search(102);
    vehicles.search(105);

    return 0;
}