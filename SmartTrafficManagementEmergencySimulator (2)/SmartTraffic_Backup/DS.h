#ifndef DS_H
#define DS_H
#include <string>

enum Direction { EAST = 0, WEST = 1, SOUTH = 2, NORTH = 3 };

struct VehicleNode {
    int id;
    std::string type;
    VehicleNode* next;
    VehicleNode(int vehicleId, const std::string& vehicleType)
        : id(vehicleId), type(vehicleType), next(nullptr) {}
};

class VehicleQueue {
private:
    VehicleNode* frontNode;
    VehicleNode* rearNode;
    int total;
public:
    VehicleQueue();
    ~VehicleQueue();
    void enqueue(int id, const std::string& type);
    bool dequeue();
    int size() const;
    bool isEmpty() const;
};
#endif
