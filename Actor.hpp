#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <vector>
#include "Edge.hpp"

using namespace std;

class Actor {

  public:
    string actorName;
    bool visited;
    int distance;
    Actor* prevActor;
    vector<Edge*> edges;

    Actor(string actorName);
};

#endif 

