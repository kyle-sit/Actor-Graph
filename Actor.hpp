#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Edge;

class Actor {
  public:
    string actorName;
    bool visited;
    int distance;
    Actor* prevActor;
    Edge* prevMovie;
    
    unordered_map<string, Edge*> movieList;

    Actor(string actorName);

    bool operator<(const Actor& other) const;
};

class ActorPtrComp {
public:
    bool operator()(Actor*& lhs, Actor*& rhs) const {
        return (*lhs).distance < (*rhs).distance;
    }
};
#endif 

