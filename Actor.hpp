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
    Edge* prevMovie;
    vector<Edge*> edges;

    Actor(string actorName){
      visited = false;
      distance = 0;
      this->actorName = actorName;
      this->prevActor = nullptr;
    }
};

#endif 

