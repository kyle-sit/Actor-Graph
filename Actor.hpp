/*
 * Actor.hpp
 * Author: Kyle Sit, Louis Leung
 * Date: 6/2/16
 *
 * This file holds design of the Actors in the Graph
 */

#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

//forward declaration 
class Edge;

//Class with member variables
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

//Used to construct a minheap instead of a PQ default maxheap
class ActorPtrComp {
public:
    bool operator()(Actor*& lhs, Actor*& rhs) const {
        return (*lhs).distance > (*rhs).distance;
    }
};
#endif 

