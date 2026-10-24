
#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>
using namespace std;

class Vehicle {
protected:
    int id;

public:
    Vehicle(int vehicleId) : id(vehicleId) {}

    virtual string getType() const = 0;

    void display() const {
        cout << "Vehicle ID: " << id
             << ", Type: " << getType() << endl;
    }

    virtual ~Vehicle() {}
};

class Car : public Vehicle {
public:
    Car(int vehicleId) : Vehicle(vehicleId) {}

    string getType() const override {
        return "Car";
    }
};

class Ambulance : public Vehicle {
public:
    Ambulance(int vehicleId) : Vehicle(vehicleId) {}

    string getType() const override {
        return "Ambulance";
    }
};

#endif
