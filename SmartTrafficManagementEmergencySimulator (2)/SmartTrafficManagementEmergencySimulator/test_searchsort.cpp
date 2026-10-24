
#include "SearchSort.h"
#include <iostream>
using namespace std;

int main() {
    int vehicleIDs[] = {105, 101, 104, 102, 103};
    int n = 5;

    cout << "Searching for Vehicle ID 104:\n";
    linearSearch(vehicleIDs, n, 104);

    bubbleSort(vehicleIDs, n);

    return 0;
}
