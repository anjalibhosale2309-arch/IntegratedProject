#ifndef VEHICLE_HASH_H
#define VEHICLE_HASH_H

const int HASH_SIZE = 10;

class VehicleHash {
private:
    int table[HASH_SIZE];

    int hashFunction(int id);

public:
    VehicleHash();
    void insert(int id);
    void search(int id);
    void display();
};

#endif