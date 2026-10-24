#ifndef OOP_H
#define OOP_H
#include "DS.h"

class TrafficController {
private:
    VehicleQueue lanes[4];
    int currentLane, emergencyLane, emergencyTicks, tickCount, nextEmergencyLane, vehicleId;
    float vehiclePosition[4];
public:
    TrafficController();
    void update();
    void triggerEmergency(int lane);
    void triggerNextEmergency();
    int getGreenLane() const;
    int getEmergencyLane() const;
    float getVehiclePosition(int lane) const;
    int getQueueSize(int lane) const;
};
#endif
