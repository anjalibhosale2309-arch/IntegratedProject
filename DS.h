// ==============================================================================
// MIT ACADEMY OF ENGINEERING, ALANDI (PUNE)
// Department of Computer Engineering
// Second Year B.Tech - Data Structures (2310212T / 2310214L)
//
// Mapped Course Outcomes:
//   - CO1: Explain the working of linear data structures & arrays [L2/L3]
//   - CO2: Apply concept of arrays for developing various applications [L3]
//   - CO3: Singly Linked List Queue (FIFO for approach lanes) [L3]
//   - CO4: Stack (LIFO event logs) and Priority Queue (Ambulance preemption) [L3]
//   - CO5: Searching & Sorting (Linear Search, Bubble Sort for vehicle registry) [L3]
//   - CO6: Hashing (Hash table with modulo arithmetic and linear probing) [L3]
// ==============================================================================

#ifndef DS_H
#define DS_H

#include <string>
#include <vector>
#include <utility>

enum Direction { EAST = 0, WEST = 1, SOUTH = 2, NORTH = 3 };

// ------------------------------------------------------------------------------
// CO3: Singly-Linked List FIFO Queue (for road approach lanes)
// ------------------------------------------------------------------------------
struct VehicleNode {
    int id;
    std::string type;
    VehicleNode* next;

    VehicleNode(int vId, const std::string& vType)
        : id(vId), type(vType), next(nullptr) {}
};

class VehicleQueue {
private:
    VehicleNode* frontNode;
    VehicleNode* rearNode;
    int count;

public:
    VehicleQueue();
    ~VehicleQueue();

    void enqueue(int id, const std::string& type);
    bool dequeue();
    int size() const { return count; }
    bool isEmpty() const { return frontNode == nullptr; }
    void clear();
    std::vector<std::pair<int, std::string>> getElements() const;
};

// ------------------------------------------------------------------------------
// CO4: Priority Queue (Ambulance gets highest priority preemption)
// ------------------------------------------------------------------------------
struct EmergencyItem {
    int id;
    int road;
    int priority; // 2 = High (Ambulance), 1 = Medium (Bus), 0 = Normal (Car)
    std::string type;
};

class EmergencyPriorityQueue {
private:
    std::vector<EmergencyItem> items;

public:
    EmergencyPriorityQueue();
    void insert(int id, int road, int priority, const std::string& type);
    bool popHighest(EmergencyItem& out);
    int size() const { return (int)items.size(); }
    bool isEmpty() const { return items.empty(); }
    void clear() { items.clear(); }
    const std::vector<EmergencyItem>& getItems() const { return items; }
};

// ------------------------------------------------------------------------------
// CO4: Traffic Event Stack (LIFO Action History / Log)
// ------------------------------------------------------------------------------
class TrafficStack {
private:
    static const int MAX_SIZE = 25;
    std::string events[MAX_SIZE];
    int top;

public:
    TrafficStack();
    bool push(const std::string& msg);
    bool pop(std::string& outMsg);
    bool peek(std::string& outMsg) const;
    int size() const { return top + 1; }
    bool isEmpty() const { return top == -1; }
    void clear() { top = -1; }
    std::vector<std::string> getAllEvents() const;
};

// ------------------------------------------------------------------------------
// CO6: Vehicle Hash Table (Modulo arithmetic & Linear Probing collision resolution)
// ------------------------------------------------------------------------------
class VehicleHash {
public:
    static const int HASH_SIZE = 10;

private:
    int table[HASH_SIZE];
    std::string types[HASH_SIZE];
    int count;

public:
    VehicleHash();
    int hashFunction(int id) const { return id % HASH_SIZE; }
    bool insert(int id, const std::string& type);
    int search(int id) const;
    bool remove(int id);
    void clear();
    int getBucket(int index) const { return table[index]; }
    std::string getType(int index) const { return types[index]; }
    int size() const { return count; }
};

// ------------------------------------------------------------------------------
// CO5: Searching & Sorting Algorithms
// ------------------------------------------------------------------------------
class SearchSortDemo {
public:
    static int linearSearch(const int arr[], int n, int key);
    static void bubbleSort(int arr[], int n);
};

#endif // DS_H
