
#ifndef VEHICLE_MANAGER_H
#define VEHICLE_MANAGER_H

#include <string>
using namespace std;

const int MAX_VEHICLES = 20;

struct Vehicle {
    int id;
    string type;
    string direction;
};

class VehicleManager {
private:
    Vehicle vehicles[MAX_VEHICLES];
    int count;

public:
    VehicleManager();

    bool addVehicle(int id, string type, string direction);
    void displayVehicles() const;
};

#endif
