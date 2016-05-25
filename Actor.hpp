#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <vector>
#include "Actor.hpp"

using namespace std;

class Actor {

public:
	string actorName;
	bool visited;
	int distance;
	Actor * prevActor;
  std::vector<Edge*> edges;

	Actor(string actorName) {
		this->actorName = actorName;
		this->prevActor = nullptr;
	}
};

#endif 

