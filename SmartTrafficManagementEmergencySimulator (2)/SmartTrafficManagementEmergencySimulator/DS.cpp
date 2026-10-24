// ==============================================================================
// MIT ACADEMY OF ENGINEERING, ALANDI (PUNE)
// Department of Computer Engineering
// Second Year B.Tech - Data Structures (2310212T / 2310214L)
// Implementation of Queues, Priority Queues, Stacks, Hash Table & Search/Sort
// ==============================================================================

#include "DS.h"
#include <algorithm>

// ------------------------------------------------------------------------------
// Singly Linked List FIFO Queue
// ------------------------------------------------------------------------------
VehicleQueue::VehicleQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

VehicleQueue::~VehicleQueue() {
    clear();
}

void VehicleQueue::clear() {
    while (dequeue()) {}
}

void VehicleQueue::enqueue(int id, const std::string& type) {
    VehicleNode* node = new VehicleNode(id, type);
    if (!rearNode) {
        frontNode = rearNode = node;
    } else {
        rearNode->next = node;
        rearNode = node;
    }
    ++count;
}

bool VehicleQueue::dequeue() {
    if (!frontNode) return false;
    VehicleNode* temp = frontNode;
    frontNode = frontNode->next;
    if (!frontNode) rearNode = nullptr;
    delete temp;
    --count;
    return true;
}

std::vector<std::pair<int, std::string>> VehicleQueue::getElements() const {
    std::vector<std::pair<int, std::string>> result;
    VehicleNode* curr = frontNode;
    while (curr) {
        result.push_back({curr->id, curr->type});
        curr = curr->next;
    }
    return result;
}

// ------------------------------------------------------------------------------
// Priority Queue Implementation (Emergency items ordered by priority)
// ------------------------------------------------------------------------------
EmergencyPriorityQueue::EmergencyPriorityQueue() {}

void EmergencyPriorityQueue::insert(int id, int road, int priority, const std::string& type) {
    items.push_back({id, road, priority, type});
    std::sort(items.begin(), items.end(), [](const EmergencyItem& a, const EmergencyItem& b) {
        return a.priority > b.priority;
    });
}

bool EmergencyPriorityQueue::popHighest(EmergencyItem& out) {
    if (items.empty()) return false;
    out = items.front();
    items.erase(items.begin());
    return true;
}

// ------------------------------------------------------------------------------
// Traffic Event Stack (LIFO History)
// ------------------------------------------------------------------------------
TrafficStack::TrafficStack() : top(-1) {}

bool TrafficStack::push(const std::string& msg) {
    if (top >= MAX_SIZE - 1) {
        for (int i = 0; i < MAX_SIZE - 1; ++i) {
            events[i] = events[i + 1];
        }
        events[MAX_SIZE - 1] = msg;
        return true;
    }
    events[++top] = msg;
    return true;
}

bool TrafficStack::pop(std::string& outMsg) {
    if (top == -1) return false;
    outMsg = events[top--];
    return true;
}

bool TrafficStack::peek(std::string& outMsg) const {
    if (top == -1) return false;
    outMsg = events[top];
    return true;
}

std::vector<std::string> TrafficStack::getAllEvents() const {
    std::vector<std::string> list;
    for (int i = top; i >= 0; --i) {
        list.push_back(events[i]);
    }
    return list;
}

// ------------------------------------------------------------------------------
// Vehicle Hash Table (Modulo + Linear Probing)
// ------------------------------------------------------------------------------
VehicleHash::VehicleHash() : count(0) {
    clear();
}

void VehicleHash::clear() {
    for (int i = 0; i < HASH_SIZE; ++i) {
        table[i] = -1;
        types[i] = "";
    }
    count = 0;
}

bool VehicleHash::insert(int id, const std::string& type) {
    if (count >= HASH_SIZE) return false;
    int index = hashFunction(id);
    int start = index;
    while (table[index] != -1 && table[index] != -2) {
        if (table[index] == id) return false; // Duplicate
        index = (index + 1) % HASH_SIZE;
        if (index == start) return false;
    }
    table[index] = id;
    types[index] = type;
    ++count;
    return true;
}

int VehicleHash::search(int id) const {
    int index = hashFunction(id);
    int start = index;
    while (table[index] != -1) {
        if (table[index] == id) return index;
        index = (index + 1) % HASH_SIZE;
        if (index == start) break;
    }
    return -1;
}

bool VehicleHash::remove(int id) {
    int idx = search(id);
    if (idx != -1) {
        table[idx] = -2; // Deleted marker
        types[idx] = "";
        --count;
        return true;
    }
    return false;
}

// ------------------------------------------------------------------------------
// Searching & Sorting Demo Implementations
// ------------------------------------------------------------------------------
int SearchSortDemo::linearSearch(const int arr[], int n, int key) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == key) return i;
    }
    return -1;
}

void SearchSortDemo::bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
