#include "VehicleHash.h"
#include <iostream>
using namespace std;

VehicleHash::VehicleHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        table[i] = -1;
    }
}

int VehicleHash::hashFunction(int id) {
    return id % HASH_SIZE;
}

void VehicleHash::insert(int id) {
    int index = hashFunction(id);

    while (table[index] != -1) {
        index = (index + 1) % HASH_SIZE;
    }

    table[index] = id;
    cout << "Vehicle ID " << id << " inserted.\n";
}

void VehicleHash::search(int id) {
    int index = hashFunction(id);
    int start = index;

    while (table[index] != -1) {
        if (table[index] == id) {
            cout << "Vehicle ID " << id
                 << " found at index " << index << ".\n";
            return;
        }

        index = (index + 1) % HASH_SIZE;

        if (index == start) {
            break;
        }
    }

    cout << "Vehicle ID " << id << " not found.\n";
}

void VehicleHash::display() {
    cout << "\n--- Vehicle Hash Table ---\n";

    for (int i = 0; i < HASH_SIZE; i++) {
        cout << i << " : ";

        if (table[i] == -1) {
            cout << "Empty";
        } else {
            cout << table[i];
        }

        cout << '\n';
    }
}