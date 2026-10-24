
#ifndef TRAFFIC_STACK_H
#define TRAFFIC_STACK_H

#include <string>
using namespace std;

const int MAX_EVENTS = 20;

class TrafficStack {
private:
    string events[MAX_EVENTS];
    int top;

public:
    TrafficStack();

    bool push(string event);
    bool pop();
    void display() const;
};

#endif
