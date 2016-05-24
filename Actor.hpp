#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>

using namespace std;

class Actor {

public:
	string actorName;
	bool visited;
	int distance;
	Actor * prevActor;

	Actor(string actorName, Actor * prevActor) {
		this->actorName = actorName;
		this->prevActor = prevActor;
	}
};

#endif 

