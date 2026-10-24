// ==============================================================================
// MIT ACADEMY OF ENGINEERING, ALANDI (PUNE)
// Department of Computer Engineering
// Second Year B.Tech - Problem Solving using OOP (2310261L)
//
// Mapped Course Outcomes:
//   - CO1: Classes, constructors, destructors, modular structure [L3]
//   - CO2: Inheritance (Vehicle abstract base -> Car, Bus, Ambulance) [L3]
//   - CO3: Runtime Polymorphism (virtual draw(), update() dynamic dispatch) [L3]
//   - CO4: Critical thinking, STL containers, dynamic memory management [L3]
// ==============================================================================

#ifndef OOP_H
#define OOP_H

#include "DS.h"
#include <string>
#include <vector>
#include <memory>

// C-linkage to real 80386/x86_64 Assembly routines in COA_Logic.asm
extern "C" {
    int check_traffic_priority(int vehicleCount, int threshold);
    int calculate_green_time(int queueSize, int isEmergency);
    int coa_multiply(int a, int b);
    int coa_evaluate_sensor(int roadMask, int emergencyMask);
}

// ------------------------------------------------------------------------------
// Abstract Base Class: Vehicle (OOP CO1, CO2, CO3)
// ------------------------------------------------------------------------------
class Vehicle {
protected:
    int id;
    int road;              // 0=East, 1=West, 2=South, 3=North
    float position;        // 0.02f (spawn) -> 0.40f (stop line) -> 1.05f (cleared)
    float speed;
    float colorR, colorG, colorB;

public:
    Vehicle(int vId, int vRoad, float r, float g, float b, float vSpeed = 0.007f);
    virtual ~Vehicle() {}

    // Pure virtual methods ensuring dynamic dispatch (CO3 Polymorphism)
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual int getPriority() const = 0;

    virtual void update(bool isGreen);

    int getId() const { return id; }
    int getRoad() const { return road; }
    float getPosition() const { return position; }
    void setPosition(float p) { position = p; }
    bool isCleared() const { return position >= 1.05f; }
};

// ------------------------------------------------------------------------------
// Subclass 1: Car
// ------------------------------------------------------------------------------
class Car : public Vehicle {
public:
    Car(int vId, int vRoad);
    void draw() const override;
    std::string getType() const override { return "Car"; }
    int getPriority() const override { return 0; }
};

// ------------------------------------------------------------------------------
// Subclass 2: Bus
// ------------------------------------------------------------------------------
class Bus : public Vehicle {
public:
    Bus(int vId, int vRoad);
    void draw() const override;
    std::string getType() const override { return "Bus"; }
    int getPriority() const override { return 1; }
};

// ------------------------------------------------------------------------------
// Subclass 3: Ambulance (Emergency Preemption Vehicle)
// ------------------------------------------------------------------------------
class Ambulance : public Vehicle {
private:
    mutable int beaconTick;

public:
    Ambulance(int vId, int vRoad);
    void draw() const override;
    void update(bool isGreen) override;
    std::string getType() const override { return "Ambulance"; }
    int getPriority() const override { return 2; } // High Priority
};

// ------------------------------------------------------------------------------
// Traffic Signal Class (OOP Encapsulation)
// ------------------------------------------------------------------------------
class TrafficSignal {
private:
    int road;
    int state;          // 0 = Red, 1 = Yellow, 2 = Green
    int timerSeconds;

public:
    TrafficSignal(int r = 0);
    void setGreen(int duration);
    void setYellow(int duration = 2);
    void setRed();
    void tick();

    int getState() const { return state; }
    int getTimer() const { return timerSeconds; }
};

// ------------------------------------------------------------------------------
// Vehicle Record Structure for Vehicle Registry
// ------------------------------------------------------------------------------
struct VehicleRecord {
    int id;
    std::string type;
    int road;
    int priority;
    int spawnSecond;
    std::string status; // "Active" or "Cleared"
};

// ------------------------------------------------------------------------------
// Traffic Controller (Coordinates OOP, DSA, and COA Assembly Engine)
// ------------------------------------------------------------------------------
class TrafficController {
private:
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::vector<VehicleRecord> registry;

    TrafficSignal signals[4];
    VehicleQueue laneQueues[4];
    EmergencyPriorityQueue emergencyQueue;
    TrafficStack eventStack;
    VehicleHash vehicleHash;

    int currentGreen;
    int yellowRoad;
    int yellowTimer;
    int emergencyRoad;
    int emergencyTimer;
    int cycleTicks;
    int nextId;
    bool isPaused;
    int uptimeSeconds;
    int emergencyEventsCount;

    // COA Assembly Function Results
    int coaPriority;
    int coaTime;
    int coaSensor;

public:
    TrafficController();

    void update();
    void triggerEmergency(int road);
    void addCar(int road);
    void addBus(int road);
    void addAmbulance(int road);
    void nextPhase();
    void setManualGreen(int road);
    void spawnRandomVehicle();
    void clearClearedRegistry();
    void togglePause() { isPaused = !isPaused; }
    void reset();
    void removeVehicleById(int id);
    void sortRegistry(int sortMode); // 0=Priority, 1=Time, 2=Type, 3=ID

    int getGreen() const { return currentGreen; }
    int getYellowRoad() const { return yellowRoad; }
    int getEmergency() const { return emergencyRoad; }
    bool isEmergencyActive() const { return emergencyRoad != -1; }
    int getSignalState(int road) const { return signals[road].getState(); }
    int getSignalTimer(int road) const { return signals[road].getTimer(); }
    int getQueue(int road) const { return laneQueues[road].size(); }
    bool getPaused() const { return isPaused; }
    int getUptime() const { return uptimeSeconds; }
    int getEmergencyCount() const { return emergencyEventsCount; }

    int getCoaPriority() const { return coaPriority; }
    int getCoaTime() const { return coaTime; }
    int getCoaSensor() const { return coaSensor; }

    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const { return vehicles; }
    const std::vector<VehicleRecord>& getRegistry() const { return registry; }
    TrafficStack& getStack() { return eventStack; }
    EmergencyPriorityQueue& getPriorityQueue() { return emergencyQueue; }
    VehicleHash& getHash() { return vehicleHash; }
};

#endif // OOP_H
