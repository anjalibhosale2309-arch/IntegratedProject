
#include "TrafficStack.h"
#include <iostream>
using namespace std;

TrafficStack::TrafficStack() {
    top = -1;
}

bool TrafficStack::push(string event) {
    if (top == MAX_EVENTS - 1) {
        // Remove the oldest event when history is full
        for (int i = 0; i < MAX_EVENTS - 1; i++) {
            events[i] = events[i + 1];
        }
        top--;
    }

    events[++top] = event;
    return true;
}

bool TrafficStack::pop() {
    if (top == -1) {
        cout << "No events to remove!\n";
        return false;
    }

    cout << "Removed event: " << events[top] << '\n';
    top--;
    return true;
}

void TrafficStack::display() const {
    cout << "\n--- Traffic Event History ---\n";

    if (top == -1) {
        cout << "No events available.\n";
        return;
    }

    for (int i = top; i >= 0; i--) {
        cout << events[i] << '\n';
    }
}
