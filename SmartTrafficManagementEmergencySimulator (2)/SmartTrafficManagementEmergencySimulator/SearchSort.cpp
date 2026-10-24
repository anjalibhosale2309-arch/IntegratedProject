
#include "SearchSort.h"
#include <iostream>
using namespace std;

void linearSearch(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            cout << "Vehicle ID " << key
                 << " found at index: " << i << '\n';
            return;
        }
    }

    cout << "Vehicle ID " << key << " not found.\n";
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }

        // Stop if the array is already sorted
        if (!swapped) {
            break;
        }
    }

    cout << "Sorted Vehicle IDs: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << '\n';
}
