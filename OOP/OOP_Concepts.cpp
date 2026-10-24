
#include "Vehicle.h"

int main() {
    Car car(101);
    Ambulance ambulance(102);

    Vehicle* v1 = &car;
    Vehicle* v2 = &ambulance;

    cout << "=== OOP Concepts Demo ===" << endl;

    v1->display();
    v2->display();

    return 0;
}
