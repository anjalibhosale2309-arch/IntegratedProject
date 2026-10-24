
#include "TrafficStack.h"

int main() {
    TrafficStack history;

    history.push("East lane: Normal traffic");
    history.push("North lane: Ambulance detected");
    history.push("North lane: Emergency signal activated");

    history.display();

    history.pop();

    history.display();

    return 0;
}
