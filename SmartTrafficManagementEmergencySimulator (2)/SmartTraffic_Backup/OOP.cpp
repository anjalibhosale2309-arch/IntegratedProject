
#include "OOP.h"

TrafficController::TrafficController()
    : currentLane(EAST),
      emergencyLane(-1),
      emergencyTicks(0),
      tickCount(0),
      nextEmergencyLane(EAST),
      vehicleId(1)
{
    // Initialize vehicle positions and queues
    for (int i = 0; i < 4; ++i)
    {
        vehiclePosition[i] = 0.0f;

        for (int j = 0; j < 5; ++j)
        {
            lanes[i].enqueue(vehicleId++, "Car");
        }
    }
}

// Trigger emergency for a specific direction
void TrafficController::triggerEmergency(int lane)
{
    if (lane < EAST || lane > NORTH)
    {
        return;
    }

    emergencyLane = lane;
    currentLane = lane;
    emergencyTicks = 100; // 5 seconds at 50 ms intervals

    // Add an ambulance to the selected lane
    lanes[lane].enqueue(vehicleId++, "Ambulance");
}

// Automatically select the next emergency direction
void TrafficController::triggerNextEmergency()
{
    triggerEmergency(nextEmergencyLane);

    nextEmergencyLane = (nextEmergencyLane + 1) % 4;
}

// Update traffic simulation
void TrafficController::update()
{
    ++tickCount;

    // Move normal vehicles
    for (int i = 0; i < 4; ++i)
    {
        vehiclePosition[i] += 0.012f;

        if (vehiclePosition[i] > 1.0f)
        {
            vehiclePosition[i] = 0.0f;
        }
    }

    // Handle emergency mode
    if (emergencyLane != -1)
    {
        --emergencyTicks;

        if (emergencyTicks <= 0)
        {
            emergencyLane = -1;
            emergencyTicks = 0;

            // Resume normal signal sequence
            currentLane = (currentLane + 1) % 4;
        }

        return;
    }

    // Automatically trigger an emergency periodically
    if (tickCount % 400 == 0)
    {
        triggerNextEmergency();
        return;
    }

    // Rotate normal traffic signals
    if (tickCount % 100 == 0)
    {
        currentLane = (currentLane + 1) % 4;

        // Remove one vehicle from the active lane queue
        lanes[currentLane].dequeue();
    }
}

// Return the currently green lane
int TrafficController::getGreenLane() const
{
    if (emergencyLane != -1)
    {
        return emergencyLane;
    }

    return currentLane;
}

// Return emergency lane, or -1 when inactive
int TrafficController::getEmergencyLane() const
{
    return emergencyLane;
}

// Return the position of normal vehicles
float TrafficController::getVehiclePosition(int lane) const
{
    if (lane < EAST || lane > NORTH)
    {
        return 0.0f;
    }

    return vehiclePosition[lane];
}

// Return queue size for a direction
int TrafficController::getQueueSize(int lane) const
{
    if (lane < EAST || lane > NORTH)
    {
        return 0;
    }

    return lanes[lane].size();
}
