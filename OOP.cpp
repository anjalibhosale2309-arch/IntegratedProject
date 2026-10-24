// ==============================================================================
// MIT ACADEMY OF ENGINEERING, ALANDI (PUNE)
// Department of Computer Engineering
// Second Year B.Tech - Problem Solving using OOP (2310261L)
// Implementation of Vehicle hierarchy, Signals, Controller & Registry
// ==============================================================================

#include "OOP.h"
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper function to draw quad
static void drawQuad(float x1, float y1, float x2, float y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Helper function to draw circle
static void drawCircle(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 24; ++i) {
        float angle = i * 2.0f * (float)M_PI / 24.0f;
        glVertex2f(cx + radius * cosf(angle), cy + radius * sinf(angle));
    }
    glEnd();
}

// ==============================================================================
// Vehicle Base Class Implementation (OOP CO1, CO2)
// ==============================================================================
Vehicle::Vehicle(int vId, int vRoad, float r, float g, float b, float vSpeed)
    : id(vId), road(vRoad), position(0.04f), speed(vSpeed),
      colorR(r), colorG(g), colorB(b) {}

void Vehicle::update(bool isGreen) {
    const float STOP_LINE = 0.38f;

    // Red/Yellow light stop line behavior
    if (!isGreen) {
        if (position < STOP_LINE && (position + speed * 2.5f) >= STOP_LINE) {
            position = STOP_LINE;
            return;
        }
        if (position >= STOP_LINE && position <= STOP_LINE + 0.02f) {
            return; // Wait at stop line until light turns green!
        }
    }

    position += speed;
}

// ==============================================================================
// Car Subclass (OOP CO2 Inheritance, CO3 Polymorphism)
// ==============================================================================
Car::Car(int vId, int vRoad)
    : Vehicle(vId, vRoad, 0.20f, 0.55f, 0.95f, 0.0075f) {}

void Car::draw() const {
    float len = 18.0f, wid = 9.0f;
    // Car main chassis
    drawQuad(-len * 0.5f, -wid * 0.5f, len * 0.5f, wid * 0.5f, colorR, colorG, colorB);
    // Cabin roof & windshield
    drawQuad(-len * 0.22f, -wid * 0.35f, len * 0.22f, wid * 0.35f, 0.85f, 0.95f, 1.0f);
    // Dual Front Headlights
    drawCircle(len * 0.45f, -wid * 0.3f, 1.8f, 1.0f, 1.0f, 0.6f);
    drawCircle(len * 0.45f,  wid * 0.3f, 1.8f, 1.0f, 1.0f, 0.6f);
    // Dual Rear Taillights
    drawQuad(-len * 0.5f, -wid * 0.35f, -len * 0.45f, -wid * 0.15f, 0.9f, 0.1f, 0.1f);
    drawQuad(-len * 0.5f,  wid * 0.15f, -len * 0.45f,  wid * 0.35f, 0.9f, 0.1f, 0.1f);
}

// ==============================================================================
// Bus Subclass (OOP CO2 Inheritance, CO3 Polymorphism)
// ==============================================================================
Bus::Bus(int vId, int vRoad)
    : Vehicle(vId, vRoad, 0.95f, 0.70f, 0.15f, 0.0055f) {}

void Bus::draw() const {
    float len = 30.0f, wid = 11.0f;
    // Bus outer body
    drawQuad(-len * 0.5f, -wid * 0.5f, len * 0.5f, wid * 0.5f, colorR, colorG, colorB);
    // Passenger windows row
    for (int i = 0; i < 4; ++i) {
        float wx = -len * 0.38f + i * (len * 0.24f);
        drawQuad(wx - 2.8f, -wid * 0.38f, wx + 2.8f, -wid * 0.15f, 0.8f, 0.9f, 1.0f);
        drawQuad(wx - 2.8f,  wid * 0.15f, wx + 2.8f,  wid * 0.38f, 0.8f, 0.9f, 1.0f);
    }
    // Front windshield
    drawQuad(len * 0.35f, -wid * 0.35f, len * 0.48f, wid * 0.35f, 0.7f, 0.85f, 0.95f);
}

// ==============================================================================
// Ambulance Subclass (Emergency Preemption Vehicle)
// ==============================================================================
Ambulance::Ambulance(int vId, int vRoad)
    : Vehicle(vId, vRoad, 0.95f, 0.95f, 0.95f, 0.0110f), beaconTick(0) {}

void Ambulance::update(bool isGreen) {
    // Emergency ambulance overrides traffic signals and moves swiftly
    position += speed;
}

void Ambulance::draw() const {
    float len = 24.0f, wid = 10.0f;
    ++beaconTick;

    // White ambulance body
    drawQuad(-len * 0.5f, -wid * 0.5f, len * 0.5f, wid * 0.5f, 0.95f, 0.95f, 0.95f);

    // Red Medical Cross Decals on Roof
    drawQuad(-len * 0.12f, -wid * 0.35f, len * 0.08f, wid * 0.35f, 0.9f, 0.1f, 0.1f);
    drawQuad(-len * 0.22f, -wid * 0.12f, len * 0.18f, wid * 0.12f, 0.9f, 0.1f, 0.1f);

    // Front Windshield
    drawQuad(len * 0.28f, -wid * 0.35f, len * 0.45f, wid * 0.35f, 0.75f, 0.88f, 0.98f);

    // Flashing Emergency Siren Beacons (Red & Blue alternating strobes)
    bool strobe = ((beaconTick / 8) % 2 == 0);
    drawCircle(-len * 0.05f, -wid * 0.25f, 3.0f, strobe ? 1.0f : 0.1f, 0.1f, strobe ? 0.1f : 1.0f);
    drawCircle(-len * 0.05f,  wid * 0.25f, 3.0f, strobe ? 0.1f : 1.0f, 0.1f, strobe ? 1.0f : 0.1f);
}

// ==============================================================================
// TrafficSignal Class Implementation
// ==============================================================================
TrafficSignal::TrafficSignal(int r)
    : road(r), state(0), timerSeconds(8) {}

void TrafficSignal::setGreen(int duration) {
    state = 2; // Green
    timerSeconds = duration;
}

void TrafficSignal::setYellow(int duration) {
    state = 1; // Yellow
    timerSeconds = duration;
}

void TrafficSignal::setRed() {
    state = 0; // Red
    timerSeconds = 0;
}

void TrafficSignal::tick() {
    if (timerSeconds > 0) --timerSeconds;
}

// ==============================================================================
// TrafficController Class Implementation
// ==============================================================================
TrafficController::TrafficController()
    : currentGreen(EAST), yellowRoad(-1), yellowTimer(0),
      emergencyRoad(-1), emergencyTimer(0), cycleTicks(0),
      nextId(101), isPaused(false), uptimeSeconds(0),
      emergencyEventsCount(0), coaPriority(0), coaTime(8), coaSensor(0) {

    for (int i = 0; i < 4; ++i) {
        signals[i] = TrafficSignal(i);
        signals[i].setRed();
    }
    signals[EAST].setGreen(8);

    // Initial default vehicles
    addCar(EAST);
    addBus(WEST);
    addCar(SOUTH);
    addCar(NORTH);

    eventStack.push("Simulation Initialized: East Lane Green");
}

void TrafficController::reset() {
    vehicles.clear();
    registry.clear();
    vehicleHash.clear();
    for (int i = 0; i < 4; ++i) {
        laneQueues[i].clear();
        signals[i].setRed();
    }
    emergencyQueue.clear();
    eventStack.clear();

    currentGreen = EAST;
    yellowRoad = -1;
    yellowTimer = 0;
    emergencyRoad = -1;
    emergencyTimer = 0;
    cycleTicks = 0;
    uptimeSeconds = 0;
    emergencyEventsCount = 0;
    signals[EAST].setGreen(8);

    addCar(EAST);
    addBus(WEST);
    addCar(SOUTH);
    addCar(NORTH);

    eventStack.push("Simulation Grid Reset Complete");
}

void TrafficController::addCar(int road) {
    if (road < 0 || road > 3) return;
    int id = nextId++;
    vehicles.push_back(std::make_shared<Car>(id, road));
    laneQueues[road].enqueue(id, "Car");
    vehicleHash.insert(id, "Car");

    registry.push_back({id, "Car", road, 0, uptimeSeconds, "Active"});

    const char* names[4] = {"East", "West", "South", "North"};
    eventStack.push("Spawned Car #" + std::to_string(id) + " on " + names[road]);
}

void TrafficController::addBus(int road) {
    if (road < 0 || road > 3) return;
    int id = nextId++;
    vehicles.push_back(std::make_shared<Bus>(id, road));
    laneQueues[road].enqueue(id, "Bus");
    vehicleHash.insert(id, "Bus");

    registry.push_back({id, "Bus", road, 1, uptimeSeconds, "Active"});

    const char* names[4] = {"East", "West", "South", "North"};
    eventStack.push("Spawned Bus #" + std::to_string(id) + " on " + names[road]);
}

void TrafficController::addAmbulance(int road) {
    triggerEmergency(road);
}

void TrafficController::triggerEmergency(int road) {
    if (road < 0 || road > 3) return;

    int id = nextId++;
    vehicles.push_back(std::make_shared<Ambulance>(id, road));

    // CO4: DSA Priority Queue insertion (Priority 2 = Emergency Ambulance)
    emergencyQueue.insert(id, road, 2, "Ambulance");
    vehicleHash.insert(id, "Ambulance");

    registry.push_back({id, "Ambulance", road, 2, uptimeSeconds, "Active"});

    emergencyRoad = road;
    emergencyTimer = 160; // ~5.5 seconds priority corridor
    ++emergencyEventsCount;

    // COA Assembly Function: calculate_green_time(queueSize, isEmergency = 1)
    coaTime = calculate_green_time(laneQueues[road].size(), 1);
    coaSensor = coa_evaluate_sensor(1 << road, 0x80);

    // Preempt traffic lights: Emergency road gets GREEN immediately, all others RED!
    for (int i = 0; i < 4; ++i) {
        if (i == road) {
            signals[i].setGreen(coaTime);
        } else {
            signals[i].setRed();
        }
    }

    const char* names[4] = {"East", "West", "South", "North"};
    eventStack.push("! EMERGENCY PREEMPTION: Ambulance on " + std::string(names[road]) + " !");
}

void TrafficController::nextPhase() {
    if (emergencyRoad != -1) return;

    // Start 2-second Yellow warning light for current lane
    yellowRoad = currentGreen;
    yellowTimer = 35; // ~1.2s yellow phase
    signals[yellowRoad].setYellow(2);

    const char* names[4] = {"East", "West", "South", "North"};
    eventStack.push("Signal Warning: " + std::string(names[yellowRoad]) + " Yellow");
}

void TrafficController::removeVehicleById(int id) {
    for (auto it = vehicles.begin(); it != vehicles.end(); ++it) {
        if ((*it)->getId() == id) {
            vehicles.erase(it);
            break;
        }
    }
    for (auto& rec : registry) {
        if (rec.id == id) {
            rec.status = "Cleared";
            break;
        }
    }
    vehicleHash.remove(id);
    eventStack.push("Manually removed Vehicle #" + std::to_string(id));
}

void TrafficController::setManualGreen(int road) {
    if (road < 0 || road > 3) return;

    // Reset any emergency override
    emergencyRoad = -1;
    emergencyTimer = 0;
    yellowRoad = -1;
    yellowTimer = 0;

    currentGreen = road;
    coaTime = calculate_green_time(laneQueues[road].size(), 0);

    for (int i = 0; i < 4; ++i) {
        if (i == road) {
            signals[i].setGreen(coaTime);
        } else {
            signals[i].setRed();
        }
    }
    cycleTicks = 0;

    const char* names[4] = {"East", "West", "South", "North"};
    eventStack.push("Manual Override: " + std::string(names[road]) + " Signal set to GREEN (" + std::to_string(coaTime) + "s)");
}

void TrafficController::spawnRandomVehicle() {
    int r = rand() % 4;
    int roll = rand() % 100;
    if (roll < 20) {
        triggerEmergency(r);
    } else if (roll < 55) {
        addBus(r);
    } else {
        addCar(r);
    }
}

void TrafficController::clearClearedRegistry() {
    for (auto it = registry.begin(); it != registry.end();) {
        if (it->status == "Cleared") {
            it = registry.erase(it);
        } else {
            ++it;
        }
    }
    eventStack.push("Cleared inactive vehicles from registry");
}

void TrafficController::sortRegistry(int sortMode) {
    // 0 = By Priority descending, 1 = By Spawn Time descending, 2 = By Type, 3 = By ID
    if (sortMode == 0) {
        std::sort(registry.begin(), registry.end(), [](const VehicleRecord& a, const VehicleRecord& b) {
            return a.priority > b.priority;
        });
    } else if (sortMode == 1) {
        std::sort(registry.begin(), registry.end(), [](const VehicleRecord& a, const VehicleRecord& b) {
            return a.spawnSecond > b.spawnSecond;
        });
    } else if (sortMode == 2) {
        std::sort(registry.begin(), registry.end(), [](const VehicleRecord& a, const VehicleRecord& b) {
            return a.type < b.type;
        });
    } else {
        std::sort(registry.begin(), registry.end(), [](const VehicleRecord& a, const VehicleRecord& b) {
            return a.id < b.id;
        });
    }
}

void TrafficController::update() {
    if (isPaused) return;

    ++cycleTicks;
    if (cycleTicks % 30 == 0) {
        ++uptimeSeconds;
    }

    // COA Assembly Function: check_traffic_priority(count, threshold=2)
    coaPriority = check_traffic_priority(laneQueues[currentGreen].size(), 2);

    // 1. Emergency Preemption Handling
    if (emergencyRoad != -1) {
        if (--emergencyTimer <= 0) {
            emergencyRoad = -1;
            // Switch smoothly to next normal lane
            currentGreen = (currentGreen + 1) % 4;
            coaTime = calculate_green_time(laneQueues[currentGreen].size(), 0);
            for (int i = 0; i < 4; ++i) {
                if (i == currentGreen) signals[i].setGreen(coaTime);
                else signals[i].setRed();
            }
            cycleTicks = 0;

            EmergencyItem item;
            emergencyQueue.popHighest(item);
            eventStack.push("Emergency corridor cleared; normal cycle restored");
        }
    }
    // 2. Yellow Warning Light Countdown
    else if (yellowRoad != -1) {
        if (--yellowTimer <= 0) {
            signals[yellowRoad].setRed();
            currentGreen = (yellowRoad + 1) % 4;
            yellowRoad = -1;

            // COA Assembly Function: calculate dynamic green time
            coaTime = calculate_green_time(laneQueues[currentGreen].size(), 0);
            signals[currentGreen].setGreen(coaTime);
            cycleTicks = 0;

            const char* names[4] = {"East", "West", "South", "North"};
            eventStack.push("Phase Shift: " + std::string(names[currentGreen]) + " Green (" + std::to_string(coaTime) + "s)");
        }
    }
    // 3. Regular Signal Phase Cycle
    else {
        if (cycleTicks % 30 == 0) {
            signals[currentGreen].tick();
        }

        // Advance to yellow warning when green signal finishes
        if (signals[currentGreen].getTimer() <= 0 || cycleTicks >= 210) {
            nextPhase();
            laneQueues[currentGreen].dequeue();
        }
    }

    // Spawn occasional traffic (controlled queue density)
    if (cycleTicks % 85 == 0 && vehicles.size() < 12) {
        int r = rand() % 4;
        if (rand() % 4 == 0) addBus(r);
        else addCar(r);
    }

    // 4. Update Vehicle Motion with realistic queuing & spacing
    for (size_t i = 0; i < vehicles.size(); ++i) {
        auto& v = vehicles[i];
        int r = v->getRoad();
        bool isGreen = (signals[r].getState() == 2) || (emergencyRoad == r);

        // Check if there is another vehicle directly ahead on the same approach
        bool blocked = false;
        float myPos = v->getPosition();
        if (myPos < 0.50f) { // Only check for stoppage before intersection
            for (size_t j = 0; j < vehicles.size(); ++j) {
                if (i == j) continue;
                auto& other = vehicles[j];
                if (other->getRoad() == r) {
                    float otherPos = other->getPosition();
                    if (otherPos > myPos && (otherPos - myPos) < 0.09f) {
                        blocked = true;
                        break;
                    }
                }
            }
        }

        if (blocked) {
            // Keep safe distance behind the vehicle ahead
            continue;
        }

        v->update(isGreen);
    }

    // 5. Remove vehicles that cleared intersection
    for (auto it = vehicles.begin(); it != vehicles.end();) {
        if ((*it)->isCleared()) {
            int cId = (*it)->getId();
            for (auto& rec : registry) {
                if (rec.id == cId) {
                    rec.status = "Cleared";
                    break;
                }
            }
            it = vehicles.erase(it);
        } else {
            ++it;
        }
    }
}
