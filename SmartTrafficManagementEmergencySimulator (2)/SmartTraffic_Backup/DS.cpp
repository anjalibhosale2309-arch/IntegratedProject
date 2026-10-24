#include "DS.h"

VehicleQueue::VehicleQueue() : frontNode(nullptr), rearNode(nullptr), total(0) {}
VehicleQueue::~VehicleQueue() { while (dequeue()) {} }

void VehicleQueue::enqueue(int id, const std::string& type) {
    VehicleNode* node = new VehicleNode(id, type);
    if (!rearNode) frontNode = rearNode = node;
    else { rearNode->next = node; rearNode = node; }
    ++total;
}

bool VehicleQueue::dequeue() {
    if (!frontNode) return false;
    VehicleNode* temp = frontNode;
    frontNode = frontNode->next;
    if (!frontNode) rearNode = nullptr;
    delete temp;
    --total;
    return true;
}
int VehicleQueue::size() const { return total; }
bool VehicleQueue::isEmpty() const { return frontNode == nullptr; }
