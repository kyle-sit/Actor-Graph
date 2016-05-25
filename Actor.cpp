#include "Actor.hpp"
#include "Edge.hpp"
#include <string>

using namespace std;

Actor::Actor(string actorName) {
		visited = false;
    distance = 0;
    this->actorName = actorName;
		this->prevActor = nullptr;
}

